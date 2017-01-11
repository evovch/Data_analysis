#define LINEWIDTH 3

void buildBlackboard(TString p_mappingFilename="Mapping_2016.txt",
					 TString nx_inFile="data/analysis_full_area_2_data1.root",
					 TString padiwa_inFile="data/thrScans_from_pairs.root",
					 TString padiwa_inFile_noise="data/thrScans_from_pairs_in_noise_win.root")
{
	// Array for the mapping
	UInt_t mapping[2][64];

	// Read the mapping from the external file
	std::ifstream ifs;
	ifs.open(p_mappingFilename.Data(), std::ifstream::in);
	UInt_t pixel, nxCh, pCh;
	unsigned int lineCounter=0;
	while (ifs >> pixel >> nxCh >> pCh) {
		//cout << pixel << "\t" << nxCh << "\t" << pCh << endl;
		mapping[0][lineCounter] = nxCh;
		mapping[1][lineCounter] = pCh;
		lineCounter++;
	}

	// --------------------------------------------------------------------------------------------
	// --------------------------------------------------------------------------------------------

	// Open input ROOT files
	TFile* nxFile = new TFile(nx_inFile, "READ");
	if (nxFile->IsZombie()) {
		cerr << "Error opening file " << nx_inFile.Data() << ". Aborting." << endl;
		return;
	}

	TFile* pFile = new TFile(padiwa_inFile, "READ");
	if (pFile->IsZombie()) {
		cerr << "Error opening file " << padiwa_inFile.Data() << ". Aborting." << endl;
		return;
	}

	TFile* pnFile = new TFile(padiwa_inFile_noise, "READ");
	if (pnFile->IsZombie()) {
		cerr << "Error opening file " << padiwa_inFile_noise.Data() << ". Aborting." << endl;
		return;
	}

	// --------------------------------------------------------------------------------------------
	// --------------------------------------------------------------------------------------------

	// Get the histograms from the input ROOT files

	// In win "A"
	TH1D* pHistos[2][64];
	TH1D* nxHistos[2][64];
	TH2D* nxTmpHisto;

	// In noise win "B"
	TH1D* pnHistos[2][64];
	TH1D* nxnHistos[2][64];
	TH2D* nxnTmpHisto;

	TString histoName;

	// nXYTER in win "A"
	histoName.Form("adcInTriggeredEventWoBaseline");
	nxTmpHisto = (TH2D*)nxFile->Get(histoName);
	if (nxTmpHisto == NULL) {
		cerr << "Error. Histogram " << histoName << " not found in " << nx_inFile.Data() << ". Aborting." << endl;
		return;
	}

	// nXYTER in noise win "B"
	histoName.Form("adcInNoiseEventWoBaseline");
	nxnTmpHisto = (TH2D*)nxFile->Get(histoName);
	if (nxnTmpHisto == NULL) {
		cerr << "Error. Histogram " << histoName << " not found in " << nx_inFile.Data() << ". Aborting." << endl;
		return;
	}


	for (UInt_t iChannel=0; iChannel<64; iChannel++)
	{
		// PADIWA in win "A"
		histoName.Form("scanCurve_ch%d", iChannel);
		pHistos[0][iChannel] = (TH1D*)pFile->Get(histoName);
		if (pHistos[0][iChannel] == NULL) {
			cerr << "Error. Histogram " << histoName << " not found in " << padiwa_inFile.Data() << ". Aborting." << endl;
			return;
		}

		histoName.Form("scanDeriv_ch%d", iChannel);
		pHistos[1][iChannel] = (TH1D*)pFile->Get(histoName);
		if (pHistos[1][iChannel] == NULL) {
			cerr << "Error. Histogram " << histoName << " not found in " << padiwa_inFile.Data() << ". Aborting." << endl;
			return;
		}

		// PADIWA in noise win "B"
		histoName.Form("scanCurve_ch%d", iChannel);
		pnHistos[0][iChannel] = (TH1D*)pnFile->Get(histoName);
		if (pnHistos[0][iChannel] == NULL) {
			cerr << "Error. Histogram " << histoName << " not found in " << padiwa_inFile.Data() << ". Aborting." << endl;
			return;
		}

		histoName.Form("scanDeriv_ch%d", iChannel);
		pnHistos[1][iChannel] = (TH1D*)pnFile->Get(histoName);
		if (pnHistos[1][iChannel] == NULL) {
			cerr << "Error. Histogram " << histoName << " not found in " << padiwa_inFile.Data() << ". Aborting." << endl;
			return;
		}

		// PADIWA in win "A"
		pHistos[0][iChannel]->Scale(1./5.);
		// PADIWA in noise win "B"
		pHistos[1][iChannel]->Scale(1./5.);

		// nXYTER in win "A"
		// Get the projection for the nXYTER and compute integral
		histoName.Form("nxSpectrum_ch%d", iChannel);
		nxHistos[0][iChannel] = nxTmpHisto->ProjectionY(histoName, iChannel+1, iChannel+1);

		histoName.Form("nXYTER spectrum channel %d", iChannel);
		nxHistos[0][iChannel]->SetTitle(histoName);
		nxHistos[0][iChannel]->Scale(1./25.);

		histoName.Form("Integral of nXYTER spectrum channel %d", iChannel);
		nxHistos[1][iChannel] = (TH1D*)nxHistos[0][iChannel]->Clone();
		nxHistos[0][iChannel]->SetTitle(histoName);

		Double_t sum=0;
		for (UInt_t iBin=nxHistos[0][iChannel]->GetNbinsX(); iBin>=1; iBin--) {
			sum += nxHistos[0][iChannel]->GetBinContent(iBin);
			nxHistos[1][iChannel]->SetBinContent(iBin, sum);
		}

		// nXYTER in noise win "B"
		// Get the projection for the nXYTER and compute integral
		histoName.Form("nxSpectrum_inNoiseWin_ch%d", iChannel);
		nxnHistos[0][iChannel] = nxnTmpHisto->ProjectionY(histoName, iChannel+1, iChannel+1);

		histoName.Form("nXYTER spectrum in noise win channel %d", iChannel);
		nxnHistos[0][iChannel]->SetTitle(histoName);
		nxnHistos[0][iChannel]->Scale(1./25.);

		histoName.Form("Integral of nXYTER spectrum in noise win channel %d", iChannel);
		nxnHistos[1][iChannel] = (TH1D*)nxnHistos[0][iChannel]->Clone();
		nxnHistos[0][iChannel]->SetTitle(histoName);

		sum=0;
		for (UInt_t iBin=nxnHistos[0][iChannel]->GetNbinsX(); iBin>=1; iBin--) {
			sum += nxnHistos[0][iChannel]->GetBinContent(iBin);
			nxnHistos[1][iChannel]->SetBinContent(iBin, sum);
		}

	}

	// --------------------------------------------------------------------------------------------
	// --------------------------------------------------------------------------------------------


	gStyle->SetOptStat(kFALSE);
	TString canvName;
	TString outPicFilename;

	TCanvas* canv[64];
	// In win "A"
	// Draw all the blackboards and export them into EPS and PNG files
	for (UInt_t iPixel=0; iPixel<64; iPixel++) {
		canvName.Form("canv_%d", iPixel);
		canv[iPixel] = new TCanvas(canvName, canvName, 1364, 796);
		canv[iPixel]->Divide(2,2);

		nxCh = mapping[0][iPixel];
		pCh = mapping[1][iPixel];

		//cout << "pixel " << iPixel+1 << "\t" << "nx " << nxCh << "\t" << "padiwa " << pCh << endl;

		// PADIWA
		canv[iPixel]->cd(2);
		pHistos[0][pCh-1]->Draw();
		pHistos[0][pCh-1]->SetLineWidth(LINEWIDTH);
		pHistos[0][pCh-1]->GetYaxis()->SetRangeUser(0., 75000./5.);
		gPad->SetGrid(1, 1);
		canv[iPixel]->cd(4);
		pHistos[1][pCh-1]->Draw();
		pHistos[1][pCh-1]->SetLineWidth(LINEWIDTH);
		gPad->SetGrid(1, 1);

		// nXYTER
		canv[iPixel]->cd(3);
		nxHistos[0][nxCh]->Draw();
		nxHistos[0][nxCh]->SetLineWidth(LINEWIDTH);
		nxHistos[0][nxCh]->GetXaxis()->SetRangeUser(0., 1000.);
		gPad->SetGrid(1, 1);
		canv[iPixel]->cd(1);
		nxHistos[1][nxCh]->Draw();
		nxHistos[1][nxCh]->SetLineWidth(LINEWIDTH);
		nxHistos[1][nxCh]->GetYaxis()->SetRangeUser(0., 75000./5.);
		nxHistos[1][nxCh]->GetXaxis()->SetRangeUser(0., 1000.);
		gPad->SetGrid(1, 1);

		outPicFilename.Form("pictures_signal_eps/blackboard_inWin_pixel%d.eps", iPixel);
		canv[iPixel]->SaveAs(outPicFilename);
		outPicFilename.Form("pictures_signal_png/blackboard_inWin_pixel%d.png", iPixel);
		canv[iPixel]->SaveAs(outPicFilename);
	}


	TCanvas* canvNoise[64];
	// In noise win "B"
	// Draw all the blackboards and export them into EPS and PNG files
	for (UInt_t iPixel=0; iPixel<64; iPixel++) {
		canvName.Form("canvNoise_%d", iPixel);
		canvNoise[iPixel] = new TCanvas(canvName, canvName, 1364, 796);
		canvNoise[iPixel]->Divide(2,2);

		nxCh = mapping[0][iPixel];
		pCh = mapping[1][iPixel];

		//cout << "pixel " << iPixel+1 << "\t" << "nx " << nxCh << "\t" << "padiwa " << pCh << endl;

		// PADIWA
		canvNoise[iPixel]->cd(2);
		pnHistos[0][pCh-1]->Draw();
		pnHistos[0][pCh-1]->SetLineWidth(LINEWIDTH);
		//pnHistos[0][pCh-1]->GetYaxis()->SetRangeUser(0., 10000./5.);
		gPad->SetGrid(1, 1);
		canvNoise[iPixel]->cd(4);
		pnHistos[1][pCh-1]->Draw();
		pnHistos[1][pCh-1]->SetLineWidth(LINEWIDTH);
		gPad->SetGrid(1, 1);

		// nXYTER
		canvNoise[iPixel]->cd(3);
		nxnHistos[0][nxCh]->Draw();
		nxnHistos[0][nxCh]->SetLineWidth(LINEWIDTH);
		nxnHistos[0][nxCh]->GetXaxis()->SetRangeUser(0., 1000.);
		gPad->SetGrid(1, 1);
		canvNoise[iPixel]->cd(1);
		nxnHistos[1][nxCh]->Draw();
		nxnHistos[1][nxCh]->SetLineWidth(LINEWIDTH);
		//nxnHistos[1][nxCh]->GetYaxis()->SetRangeUser(0., 10000./5.);
		nxnHistos[1][nxCh]->GetXaxis()->SetRangeUser(0., 1000.);
		gPad->SetGrid(1, 1);

		outPicFilename.Form("pictures_noise_eps/blackboard_inNoiseWin_pixel%d.eps", iPixel);
		canvNoise[iPixel]->SaveAs(outPicFilename);
		outPicFilename.Form("pictures_noise_png/blackboard_inNoiseWin_pixel%d.png", iPixel);
		canvNoise[iPixel]->SaveAs(outPicFilename);
	}


	// --------------------------------------------------------------------------------------------
	// --------------------------------------------------------------------------------------------

	// Export all blackboards into the ROOT file

	TFile* outFile = new TFile("Blackboard_Nov2016.root", "RECREATE");
	outFile->mkdir("Canvases_inWin");
	outFile->cd("Canvases_inWin");
	for (UInt_t iPixel=0; iPixel<64; iPixel++) {
		canv[iPixel]->Write();
	}
	outFile->cd("..");

	outFile->mkdir("Histos_inWin");
	outFile->cd("Histos_inWin");
	for (UInt_t iPixel=0; iPixel<64; iPixel++) {
		nxCh = mapping[0][iPixel];
		pCh = mapping[1][iPixel];
		nxHistos[1][nxCh]->Write();
		pHistos[0][pCh-1]->Write();
		nxHistos[0][nxCh]->Write();
		pHistos[1][pCh-1]->Write();
	}
	outFile->cd("..");

	outFile->mkdir("Canvases_inNoiseWin");
	outFile->cd("Canvases_inNoiseWin");
	for (UInt_t iPixel=0; iPixel<64; iPixel++) {
		canvNoise[iPixel]->Write();
	}
	outFile->cd("..");

	outFile->mkdir("Histos_inNoiseWin");
	outFile->cd("Histos_inNoiseWin");
	for (UInt_t iPixel=0; iPixel<64; iPixel++) {
		nxCh = mapping[0][iPixel];
		pCh = mapping[1][iPixel];
		nxnHistos[1][nxCh]->Write();
		pnHistos[0][pCh-1]->Write();
		nxnHistos[0][nxCh]->Write();
		pnHistos[1][pCh-1]->Write();
	}
	outFile->cd("..");
}
