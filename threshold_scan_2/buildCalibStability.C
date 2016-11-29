void buildCalibStability(TString listFilename="calibStabilityFilelist.txt", TString fullCalibFile="calibration_files/calib_sum.root")
{
	// Read the table from the file
	std::ifstream ifs;
	ifs.open(listFilename.Data(), std::ifstream::in);

	std::string line;

	TH1D* sourceHistos[7];

	TString histoName("TDC0010/CalTableMinusLinear_0010_01");

	unsigned int lineCounter=0;
	while (ifs >> line) {
		TString curFilename;
		curFilename.Form("calibration_files/%s", line.c_str());

		TFile* curFile = new TFile(curFilename, "READ");
		if (curFile->IsZombie()) {
			cerr << "Error opening file " << curFilename.Data() << ". Aborting." << endl;
			return;
		}

		TH1D* curHisto;
		curHisto = (TH1D*)curFile->Get(histoName);
		if (curHisto == NULL) {
			cerr << "Input histogram not found. Aborting." << endl;
			return;
		}
		sourceHistos[lineCounter] = curHisto;

		lineCounter++;
	}

	TH1D* fullCalibHisto;

	TFile* curFile = new TFile(fullCalibFile, "READ");
	if (curFile->IsZombie()) {
		cerr << "Error opening file " << fullCalibFile.Data() << ". Aborting." << endl;
		return;
	}
	fullCalibHisto = (TH1D*)curFile->Get(histoName);
	if (fullCalibHisto == NULL) {
		cerr << "Input histogram not found. Aborting." << endl;
		return;
	}

	for (UInt_t i=0; i<7; i++) {
		sourceHistos[i]->Add(fullCalibHisto, -1.);
	}

	Color_t v_colors[7];
	v_colors[0] = kBlack;
	v_colors[1] = kRed;
	v_colors[2] = kYellow;
	v_colors[3] = kGreen;
	v_colors[4] = kBlue;
	v_colors[5] = kCyan;
	v_colors[6] = kMagenta;

	UInt_t v_lineStyles[7];
	v_lineStyles[0] = 1;
	v_lineStyles[1] = 2;
	v_lineStyles[2] = 3;
	v_lineStyles[3] = 4;
	v_lineStyles[4] = 5;
	v_lineStyles[5] = 6;
	v_lineStyles[6] = 7;

	TCanvas* canv = new TCanvas("canv", "canv");

	sourceHistos[0]->Draw();
	sourceHistos[0]->SetLineColor(v_colors[0]);
	sourceHistos[0]->SetLineStyle(v_lineStyles[0]);
	sourceHistos[0]->SetLineWidth(2.);

	sourceHistos[0]->GetYaxis()->SetRangeUser(-0.025, 0.025);
	sourceHistos[0]->GetXaxis()->SetRangeUser(0., 600.);
	gPad->SetGrid(1,1);
	gStyle->SetOptStat(kFALSE);
	sourceHistos[0]->SetTitle("CalibrationStability");

	for (UInt_t i=1; i<7; i++) {
		sourceHistos[i]->Draw("sames][");
		sourceHistos[i]->SetLineColor(v_colors[i]);
		sourceHistos[i]->SetLineStyle(v_lineStyles[i]);
		sourceHistos[i]->SetLineWidth(2.);
	}


}