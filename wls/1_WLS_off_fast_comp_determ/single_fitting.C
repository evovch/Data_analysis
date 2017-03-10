void single_fitting(TString inputFile="../WLSoff_analysis_info_sum_rings_calib_corr.root", Double_t xLow = 0.4, Double_t xUp = 3.5)
{
	TFile* f = new TFile(inputFile, "READ");

	// TODO - choose which histo to fit (_0 , _1 , _2 or _3)
	TH1D* histo = (TH1D*)(f->Get("fhDiffFirstInTimeOnePMTGood_3"));

	// TODO - comment if no rebinning required
	histo->Rebin();

	TF1* func = new TF1("func", "[0]*exp(-x/[1])", xLow, xUp);
	func->SetLineWidth(3);

	// TODO - set initial parameters' values
	// second, forth and sixth and eight's are the tau's
	func->SetParameters(1., 0.5);

	TCanvas* canv = new TCanvas("canv", "canv", 50, 50, 1364, 796);

	histo->GetXaxis()->SetRangeUser(0., 15.);
	histo->Fit("func", "", "", xLow, xUp);

	histo->SetLineWidth(3);
	histo->GetYaxis()->SetTitleOffset(0.8);
	histo->GetYaxis()->SetTitleSize(0.06);
	histo->GetYaxis()->SetLabelSize(0.05);
	histo->GetXaxis()->SetTitleOffset(0.8);
	histo->GetXaxis()->SetTitleSize(0.06);
	histo->GetXaxis()->SetLabelSize(0.05);


	canv->SetLogy();
	gPad->SetGrid(1, 1);

	canv->SaveAs("WLS_off_fitting.eps");
}
