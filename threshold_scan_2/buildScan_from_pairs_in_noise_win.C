#define BASELINE 0x80e0

void buildScan_from_pairs_in_noise_win(TString listFilename="analysis_results/filelist3.txt", TString picturesDir="scans/pictures_from_pairs_in_noise_win")
{
	// Read the table from the file
	std::ifstream ifs;
	ifs.open(listFilename.Data(), std::ifstream::in);

	std::string line;

	TH1D* scanCurve[64];
	TH1D* derivCurve[64];
	TString hName;
	TString hTitle;

	UInt_t totalNbins = (BASELINE-0x7600)/0x20;

	for (unsigned int i=0; i<64; i++) {
		hName.Form("scanCurve_ch%d", i);
		hTitle.Form("Threshold scan for channel %d;Theshold in a.u.;Hits", i);
		scanCurve[i] = new TH1D(hName, hTitle, totalNbins, -BASELINE, -0x7600);

		hName.Form("scanDeriv_ch%d", i);
		hTitle.Form("Derivative of the threshold scan for channel %d;Theshold in a.u.", i);
		derivCurve[i] = new TH1D(hName, hTitle, totalNbins, -BASELINE, -0x7600);
	}

	Double_t bin71value[64];
	Double_t bin74value[64];

	unsigned int lineCounter=0;
	while (ifs >> line) {
		TString curFilename;
		curFilename.Form("analysis_results/%s", line.c_str());

		TFile* curFile = new TFile(curFilename, "READ");
		if (curFile->IsZombie()) {
			cerr << "Error opening file " << curFilename.Data() << ". Aborting." << endl;
			return;
		}

		TString histoName("fhPairsPerChannelInNoiseWin");
		TH1D* curHisto;
		curHisto = (TH1D*)curFile->Get(histoName);
		if (curHisto == NULL) {
			cerr << "Input histogram not found. Aborting." << endl;
			return;
		}

		for (unsigned int i=0; i<64; i++) {
			unsigned int padiwa = i/16;
			unsigned int chInPadiwa = i%16;
			unsigned int k;

			k = i;

			scanCurve[i]->SetBinContent(totalNbins-lineCounter, curHisto->GetBinContent(k+1));

			if (lineCounter == 71) { bin71value[i] = curHisto->GetBinContent(k+1); }
			if (lineCounter == 74) { bin74value[i] = curHisto->GetBinContent(k+1); }
		}

		lineCounter++;
	}

	// Fix two bad measurements
	for (unsigned int i=0; i<64; i++) {
		scanCurve[i]->SetBinContent(totalNbins-72, bin71value[i] + (bin74value[i]-bin71value[i])/3.);
		scanCurve[i]->SetBinContent(totalNbins-73, bin71value[i] + (bin74value[i]-bin71value[i])*2./3.);
		scanCurve[i]->SetBinContent(totalNbins-76, 0.);
	}

	// Create derivative
	for (unsigned int i=0; i<64; i++) {
		for (unsigned int iBin=2; iBin<scanCurve[i]->GetNbinsX(); iBin++) {
			Double_t diff = -(scanCurve[i]->GetBinContent(iBin) - scanCurve[i]->GetBinContent(iBin-1));
			if (diff > 0) {
				derivCurve[i]->SetBinContent(iBin-1, diff);
			}
		}
	}

	// Draw into PNG and EPS files
	gStyle->SetOptStat(kFALSE);

	TCanvas* canv[64];
	for (unsigned int iCanv=0; iCanv<64; iCanv++) {
		hName.Form("canvas_%d", iCanv);
		hTitle.Form("canvas_%d", iCanv);
		canv[iCanv] = new TCanvas(hName, hTitle, 1364, 796);
		scanCurve[iCanv]->Draw();
		scanCurve[iCanv]->SetLineWidth(2);
		gPad->SetGrid(1, 1);
		//scanCurve[iCanv]->GetYaxis()->SetRangeUser(0., 65000.);

		TString outFilename;
		outFilename.Form("%s_png/ThresholdScan_ch%d.png", picturesDir.Data(), iCanv);
		canv[iCanv]->SaveAs(outFilename);
		outFilename.Form("%s_eps/ThresholdScan_ch%d.eps", picturesDir.Data(), iCanv);
		canv[iCanv]->SaveAs(outFilename);
	}


	// Export into a ROOT file
	TFile* thrScansFile = new TFile("scans/thrScans_from_pairs_in_noise_win.root", "RECREATE");
	for (unsigned int i=0; i<64; i++) {
		scanCurve[i]->Write();
		derivCurve[i]->Write();
	}

}
