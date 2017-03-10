void fitting_shift_1ns(TString WLSon_inputFile="../WLSon_analysis_sum_rings_calib_corr.root", TString WLSoff_inputFile="../WLSoff_analysis_info_sum_rings_calib_corr.root")
{
	TFile* file_WLSon = new TFile(WLSon_inputFile, "READ");
	TFile* file_WLS_off = new TFile(WLSoff_inputFile, "READ");

	// TODO - choose which histo to fit (_0 , _1 , _2 or _3)
	TH1D* histo_WLSon = (TH1D*)(file_WLSon->Get("fhDiffFirstInTimeOnePMTGood_3"));
	TH1D* histo_WLSoff = (TH1D*)(file_WLS_off->Get("fhDiffFirstInTimeOnePMTGood_3"));


	// TODO - comment if no rebinning required
	histo_WLSon->Rebin();
	histo_WLSoff->Rebin();

/*	TCanvas* canv_WLSon = new TCanvas("canv_WLSon", "canv_WLSon", 50, 50, 1500, 900);
	histo_WLSon->Draw();
	TCanvas* canv_WLSoff = new TCanvas("canv_WLSoff", "canv_WLSoff", 50, 50, 1500, 900);
	histo_WLSoff->Draw();*/

	TCanvas* canv = new TCanvas("canv", "canv", 50, 50, 1500, 900);
	histo_WLSon->SetStats(kFALSE);
	histo_WLSoff->SetStats(kFALSE);
	histo_WLSon->GetXaxis()->SetRangeUser(0., 50.);
	histo_WLSoff->GetXaxis()->SetRangeUser(0., 50.);
	gPad->SetGrid(1, 1);
	gPad->SetLogy();

	histo_WLSon->SetLineColor(kRed);
	histo_WLSon->Draw();
	gPad->Update();
	gPad->Modified();

	histo_WLSoff->Scale(2.0966);

	histo_WLSoff->SetLineColor(kBlue);
	histo_WLSoff->Draw("][sames");
	gPad->Update();
	gPad->Modified();

	// Difference

	TH1D* hDiff = (TH1D*)histo_WLSon->Clone();
	hDiff->Add(histo_WLSoff, -1.);
	hDiff->SetLineColor(kBlack);
	hDiff->SetLineWidth(2);

	// If the 1st component is 1.1ns the fitting result is better

	// TODO - choose the fitting region
	Double_t xLow = 1.5;    // ns
	Double_t xUp = 50.;      // ns
	TF1* func = new TF1("func", "[0]*exp(-(x)/1.1) + [1]*exp(-(x)/3.8) + [2]*exp(-(x)/45)", xLow, xUp);

	func->SetLineWidth(3);

	// TODO - set initial parameters' values
	func->SetParameters(1., 1., 1.);
	TCanvas* canv2 = new TCanvas("canv2", "canv2", 50, 50, 1500, 900);
	hDiff->Fit("func", "", "", xLow, xUp);

	hDiff->GetYaxis()->SetRangeUser(0.1, 3000);
	hDiff->SetLineWidth(3);

	gPad->SetGrid(1, 1);
	gPad->SetLogy();

//	canv2->SaveAs("fitting_shift_0ns_tau1_1.1ns.eps");

}
