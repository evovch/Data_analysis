void drawStability(TString listFilename="filelist.txt", TString fullFilename="../calibration_files/calib_sum7.root")
{
	// Read the table from the file
	std::ifstream ifs;
	ifs.open(listFilename.Data(), std::ifstream::in);

	std::string line;

	TFile* curFile;

	TH1D* histos[7];
	TH1D* fullHisto;

	unsigned int lineCounter=0;
	while (ifs >> line) {
		TString curFilename;
		curFilename.Form("../calibration_files/%s", line.c_str());

		curFile = new TFile(curFilename, "READ");
		if (curFile->IsZombie()) {
			cerr << "Error opening file " << curFilename.Data() << ". Aborting." << endl;
			return;
		}

		TString histoName("TDC0010/CalTable_0010_01");
		histos[lineCounter] = (TH1D*)curFile->Get(histoName);
		if (histos[lineCounter] == NULL) {
			cerr << "Input histogram not found. Aborting." << endl;
			return;
		}

		lineCounter++;
	}

	curFile = new TFile(fullFilename, "READ");
	if (curFile->IsZombie()) {
		cerr << "Error opening file " << fullFilename.Data() << ". Aborting." << endl;
		return;
	}

	TString histoName("TDC0010/CalTable_0010_01");
	fullHisto = (TH1D*)curFile->Get(histoName);
	if (fullHisto == NULL) {
		cerr << "Input histogram not found. Aborting." << endl;
		return;
	}

	for (UInt_t i=0; i<7; i++) {
		histos[i]->Add(fullHisto, -1.);
	}

/*	TCanvas* canv[7];
	TString canvName;
	canvName.Form("canv%d", i);
	canv[i] = new TCanvas(canvName);
	histos[i]->Draw();
*/

	TCanvas* canvFinal = new TCanvas("canvFinal", "", 1364, 796);

	gStyle->SetOptStat(kFALSE);

	histos[0]->Draw();
	histos[0]->SetTitle("Calibration stability");
	histos[0]->GetYaxis()->SetTitle("Fine time diff, [ns]");

	histos[0]->GetXaxis()->SetRangeUser(0., 600.);
	histos[0]->GetYaxis()->SetRangeUser(-0.03, 0.03);

	gPad->SetGrid(1, 1);

	//histos[1]->Draw("sames][");
	//histos[2]->Draw("sames][");
	histos[3]->Draw("sames][");
	//histos[4]->Draw("sames][");
	//histos[5]->Draw("sames][");
	histos[6]->Draw("sames][");

	for (UInt_t i=0; i<7; i++) {
		histos[i]->SetLineColor(kBlack);
		histos[i]->SetLineWidth(3);
	}

	histos[0]->SetLineStyle(1);
	histos[3]->SetLineStyle(2);
	histos[6]->SetLineStyle(3);

	canvFinal->SaveAs("calibrationStability_dec2016.eps");
	canvFinal->SaveAs("calibrationStability_dec2016.png");
}
