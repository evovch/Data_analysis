void fitting_WLSon(TString WLSon_inputFile="../WLSon_analysis_sum_rings_calib_corr.root", UInt_t xLowPs = 300)
{
	Double_t xLow = (Double_t)xLowPs/1000.;

	Double_t titleFontSize = 0.050;
	Double_t labelFontSize = 0.050;

	TFile* file_WLSon = new TFile(WLSon_inputFile, "READ");

	// TODO - choose which histo to fit (_0 , _1 , _2 or _3)
	TH1D* histo_WLSon = (TH1D*)(file_WLSon->Get("fhDiffFirstInTimeOnePMTGood_3"));

	// TODO - comment if no rebinning required
	histo_WLSon->Rebin();

	TCanvas* canv = new TCanvas("canv", "canv", 50, 50, 1700, 900);

	histo_WLSon->SetStats(kFALSE);
	histo_WLSon->SetLineColor(15);
	histo_WLSon->SetLineWidth(3);
	histo_WLSon->SetTitle("");

	histo_WLSon->GetXaxis()->SetRangeUser(0., 50.);
	histo_WLSon->GetXaxis()->SetLabelSize(labelFontSize);
	histo_WLSon->GetXaxis()->SetTitle("ns    ");
	histo_WLSon->GetXaxis()->SetTitleSize(titleFontSize);
	histo_WLSon->GetXaxis()->SetTitleOffset(0.6);

	histo_WLSon->GetYaxis()->SetRangeUser(1., 20000.);
	histo_WLSon->GetYaxis()->SetLabelSize(labelFontSize);
	histo_WLSon->GetYaxis()->SetTitleSize(titleFontSize);
	histo_WLSon->GetYaxis()->SetTitleOffset(0.6);

	histo_WLSon->Draw();

	gPad->SetGrid(1, 1);
	gPad->SetLogy();


	// TODO - choose the fitting region
	Double_t xUp = 50.;      // ns
	TF1* func = new TF1("func", "[0]*exp(-x/0.54) + [1]*exp(-x/1.4) + [2]*exp(-x/3.8) + [3]*exp(-x/45)", xLow, xUp);
	func->SetLineWidth(2);

	// TODO - set initial parameters' values
	func->SetParameters(1., 1., 1., 1.);
	histo_WLSon->Fit("func", "", "", xLow, xUp);

	printf ("0.540ns: %f\n", (0.54/3.8)*func->GetParameter(0)/(func->GetParameter(2)));
	printf ("1.400ns: %f\n", (1.4/3.8)*func->GetParameter(1)/(func->GetParameter(2)));
	printf ("3.800ns: %f\n", (3.8/3.8)*func->GetParameter(2)/(func->GetParameter(2)));
	printf ("45.00ns: %f\n", (45.0/3.8)*func->GetParameter(3)/(func->GetParameter(2)));

	TBox* rect1 = new TBox(25., 100., 45., 10000);
	rect1->Draw();

	TString paramtext;

	paramtext.Form("0.540ns: %f\n", (0.54/3.8)*func->GetParameter(0)/(func->GetParameter(2)));
	TText *t1 = new TText(26., 4000., paramtext.Data());
	t1->Draw();

	paramtext.Form("1.400ns: %f\n", (1.4/3.8)*func->GetParameter(1)/(func->GetParameter(2)));
	TText *t2 = new TText(26., 1500., paramtext.Data());
	t2->Draw();

	paramtext.Form("3.800ns: %f\n", (3.8/3.8)*func->GetParameter(2)/(func->GetParameter(2)));
	TText *t3 = new TText(26., 400., paramtext.Data());
	t3->Draw();

	paramtext.Form("45.00ns: %f\n", (45.0/3.8)*func->GetParameter(3)/(func->GetParameter(2)));
	TText *t4 = new TText(26., 150., paramtext.Data());
	t4->Draw();


	TString outfileName;
	outfileName.Form("pictures_1_4ns/fitting_WLSon_4comp_1_1ns_start_%0.3fns.eps", xLow);
	canv->SaveAs(outfileName);
	outfileName.Form("pictures_1_4ns/fitting_WLSon_4comp_1_1ns_start_%0.3fns.png", xLow);
	canv->SaveAs(outfileName);
	//cout << outfileName.Data() << endl;
}
