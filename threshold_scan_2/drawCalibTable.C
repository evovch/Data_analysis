void drawCalibTable(TString p_filename="calibration_files/calib_sum.root", TString p_filename2="globalCalibFitting.root")
{

	TFile* curFile = new TFile(p_filename, "READ");
	if (curFile->IsZombie()) {
		cerr << "Error opening file " << p_filename.Data() << ". Aborting." << endl;
		return;
	}

	TString histoName("TDC0010/CalTable_0010_01");
	TH1D* histo1;
	histo1 = (TH1D*)curFile->Get(histoName);
	if (histo1 == NULL) {
		cerr << "Input histogram not found. Aborting." << endl;
		return;
	}

	TString histoName2("TDC0010/CalTableMinusLinear_0010_01");
	TH1D* histo2;
	histo2 = (TH1D*)curFile->Get(histoName2);
	if (histo2 == NULL) {
		cerr << "Input histogram not found. Aborting." << endl;
		return;
	}

	gStyle->SetOptStat(kFALSE);

	TCanvas* canv1 = new TCanvas("canv1", "", 1364, 796);

	histo1->Draw();
	histo1->SetLineWidth(2);
	histo1->GetXaxis()->SetRangeUser(0., 600.);
	gPad->SetGrid(1, 1);

//	canv1->SaveAs("CalTable_0010_01.eps");

	TCanvas* canv2 = new TCanvas("canv2", "", 1364, 796);

	histo2->Draw();
	histo2->SetLineWidth(2);
	histo2->GetXaxis()->SetRangeUser(0., 600.);
	gPad->SetGrid(1, 1);

//	canv2->SaveAs("CalTableMinusLinear_0010_01.eps");

	//--------------------------------------------------------------------------------

	TFile* curFile2 = new TFile(p_filename2, "READ");
	if (curFile2->IsZombie()) {
		cerr << "Error opening file " << p_filename2.Data() << ". Aborting." << endl;
		return;
	}

	TString histoName3("TDC0010/CalibTableMinusFit_TDC0010_CH1");
	TH1D* histo3;
	histo3 = (TH1D*)curFile2->Get(histoName3);
	if (histo3 == NULL) {
		cerr << "Input histogram not found. Aborting." << endl;
		return;
	}

	TCanvas* canv3 = new TCanvas("canv3", "", 1364, 796);

	histo3->Draw();
	histo3->SetLineWidth(2);
	histo3->GetXaxis()->SetRangeUser(0., 600.);
	histo3->GetXaxis()->SetTitle("Fine time counter value");
	histo3->GetYaxis()->SetTitle("Fine time diff, [ns]");
	gPad->SetGrid(1, 1);

//	canv3->SaveAs("CalTableMinusFit_0010_01.eps");
}
