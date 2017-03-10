#define INDEX 11

void fitAll(TString filename="scans/thrScans_from_pairs.root", TString nxFilename="analysis_full_area_data1_0000.root")
{
	TH1D* thrScan[64];
	TH1D* thrScanDeriv[64];

	// Read threshold scans from the external file
	TFile* inputFile = new TFile(filename, "READ");
	if (inputFile->IsZombie()) {
		cerr << "Error opening file " << filename.Data() << ". Aborting." << endl;
		return;
	}

	TString histoName;

	for (UInt_t i=0; i<64; i++) {
		histoName.Form("scanCurve_ch%d", i);
		thrScan[i] = (TH1D*)inputFile->Get(histoName);
		if (thrScan[i] == NULL) {
			cerr << "Error. Histogram " << histoName << " not found in " << filename.Data() << ". Aborting." << endl;
			return;
		}

		histoName.Form("scanDeriv_ch%d", i);
		thrScanDeriv[i] = (TH1D*)inputFile->Get(histoName);
		if (thrScanDeriv[i] == NULL) {
			cerr << "Error. Histogram " << histoName << " not found in " << filename.Data() << ". Aborting." << endl;
			return;
		}
	}

	// Pre-scale the PADIWA histograms
	thrScan[INDEX]->Scale(1./5.);
	thrScanDeriv[INDEX]->Scale(1./5.);

	// Fit the scan
	TF1* polynom = new TF1("polynom", "[0] + [1]*(x+32640) + [2]*(x+32640)**2 + [3]*(x+32640)**3 + [4]*(x+32640)**4 + [5]*(x+32640)**5 + [6]*(x+32640)**6 + [7]*(x+32640)**7", -0x7f80, -0x7600);
	polynom->SetLineWidth(3);
	//FIXME - tune it!
	TVirtualFitter::SetMaxIterations(4000);
	cout << "Default precision: " << TVirtualFitter::GetPrecision() << endl;
	TVirtualFitter::SetPrecision(1e-2);
	cout << "Forced precision: " << TVirtualFitter::GetPrecision() << endl;
	thrScan[INDEX]->Fit(polynom, "R");

	// Find and draw the derivative

	// Analytical function - derivative of the fitting curve
	TF1* myDeriv = new TF1("myDeriv", "-33*([0] + 2*[1]*(x+32640) + 3*[2]*(x+32640)**2 + 4*[3]*(x+32640)**3 + 5*[4]*(x+32640)**4 + 6*[5]*(x+32640)**5 + 7*[6]*(x+32640)**6)", -0x7f80, -0x7600);
	myDeriv->SetParameters(polynom->GetParameter(1),
							polynom->GetParameter(2),
							polynom->GetParameter(3),
							polynom->GetParameter(4),
							polynom->GetParameter(5),
							polynom->GetParameter(6),
							polynom->GetParameter(7));

	//thrScanDeriv[INDEX]->Scale(0.03);

	// Draw -------------------------------------------------------------------

	TCanvas* canv = new TCanvas("canv", "canv", 1364, 796);
	gStyle->SetOptStat(kFALSE);
	canv->Divide(2,2);

	canv->cd(4); gPad->SetGrid(1, 1);
	thrScan[INDEX]->SetTitle("D");
	thrScan[INDEX]->GetYaxis()->SetRangeUser(0., 16000.);
	thrScan[INDEX]->SetLineWidth(2);
	thrScan[INDEX]->SetLineColor(kBlack);
	thrScan[INDEX]->Draw();

	canv->cd(2); gPad->SetGrid(1, 1);
	thrScanDeriv[INDEX]->SetTitle("B");
	thrScanDeriv[INDEX]->SetLineWidth(2);
	thrScanDeriv[INDEX]->SetLineColor(kBlack);
	thrScanDeriv[INDEX]->Draw();

	myDeriv->SetTitle("Derivative of the threshold scan");
	myDeriv->GetXaxis()->SetTitle("Threshold in a.u.");
	myDeriv->SetLineWidth(3);
	myDeriv->SetLineColor(kRed);
	myDeriv->Draw("sames");


	// n-XYTER ----------------------------------------------------------------

	TFile* inputFileNx = new TFile(nxFilename, "READ");
	if (inputFileNx->IsZombie()) {
		cerr << "Error opening file " << nxFilename.Data() << ". Aborting." << endl;
		return;
	}

	histoName = "TrigEventBuilding/adcInTriggeredEventWoBaseline";
	TH2D* nxTmpHisto = (TH2D*)inputFileNx->Get(histoName);
	if (nxTmpHisto == NULL) {
		cerr << "Error. Histogram " << histoName << " not found in " << nxFilename.Data() << ". Aborting." << endl;
		return;
	}

	UInt_t iChannel = 9;

	histoName.Form("nxSpectrum_ch%d", iChannel);
	TH1D* nxHisto1 = nxTmpHisto->ProjectionY(histoName, iChannel+1, iChannel+1);
	TH1D* nxHisto2 = (TH1D*)nxHisto1->Clone();

	Double_t sum=0;
	for (UInt_t iBin=nxHisto1->GetNbinsX(); iBin>=1; iBin--) {
		sum += nxHisto1->GetBinContent(iBin);
		nxHisto2->SetBinContent(iBin, sum);
	}

	// Draw -------------------------------------------------------------------

	canv->cd(1); gPad->SetGrid(1, 1);
	nxHisto1->SetTitle("A");
	nxHisto1->GetYaxis()->SetTitle("Entries");
	nxHisto1->GetXaxis()->SetRangeUser(0., 1000.);
	nxHisto1->SetLineWidth(2);
	nxHisto1->SetLineColor(kBlack);
	nxHisto1->Draw();

	canv->cd(3); gPad->SetGrid(1, 1);
	histoName.Form("Integral of nXYTER spectrum channel %d", iChannel);
	nxHisto2->SetTitle("C"); // histoName
	nxHisto2->GetXaxis()->SetRangeUser(0., 1000.);
	nxHisto2->GetYaxis()->SetRangeUser(0., 16000.);
	nxHisto2->SetLineWidth(2);
	nxHisto2->SetLineColor(kBlack);
	nxHisto2->Draw();


	Double_t fontSize = 0.055;
	nxHisto2->GetXaxis()->SetTitleSize(fontSize);
	nxHisto2->GetYaxis()->SetTitleSize(fontSize);
	nxHisto1->GetXaxis()->SetTitleSize(fontSize);
	nxHisto1->GetYaxis()->SetTitleSize(fontSize);
	thrScan[INDEX]->GetXaxis()->SetTitleSize(fontSize);
	thrScan[INDEX]->GetYaxis()->SetTitleSize(fontSize);
	thrScanDeriv[INDEX]->GetXaxis()->SetTitleSize(fontSize);
	thrScanDeriv[INDEX]->GetYaxis()->SetTitleSize(fontSize);

	nxHisto2->GetXaxis()->SetLabelSize(fontSize);
	nxHisto2->GetYaxis()->SetLabelSize(fontSize);
	nxHisto1->GetXaxis()->SetLabelSize(fontSize);
	nxHisto1->GetYaxis()->SetLabelSize(fontSize);
	thrScan[INDEX]->GetXaxis()->SetLabelSize(fontSize);
	thrScan[INDEX]->GetYaxis()->SetLabelSize(fontSize);
	thrScanDeriv[INDEX]->GetXaxis()->SetLabelSize(fontSize);
	thrScanDeriv[INDEX]->GetYaxis()->SetLabelSize(fontSize);

	Double_t titleOffset = 0.82;

	nxHisto2->GetXaxis()->SetTitleOffset(titleOffset);
	nxHisto1->GetXaxis()->SetTitleOffset(titleOffset);
	thrScan[INDEX]->GetXaxis()->SetTitleOffset(titleOffset);
	thrScanDeriv[INDEX]->GetXaxis()->SetTitleOffset(titleOffset);

	thrScan[INDEX]->GetYaxis()->SetTitleOffset(1.04);

	canv->SaveAs("Blackboard_4feb.eps");
	canv->SaveAs("Blackboard_4feb.png");

/*
	TFile* outputFile = new TFile("fittedScans.root", "RECREATE");
	for (UInt_t i=0; i<64; i++) {
		thrScan[i]->SetStats(kFALSE);
		thrScan[i]->Write();
	}
*/
}
