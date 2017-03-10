/*

3.75	(tau = 1.1 ns)
1		(tau = 3.8 ns)
0.019	(tau = 45 ns)

Michael

5.15	(tau = 1.4 ns)
1		(tau = 3.8 ns)
0.07	(tau = 45 ns)

*/

void drawFunc()
{
	TF1* func = new TF1("func", "10*([0]*exp(-(x-1.0)/[1]) + [2]*exp(-(x-1.0)/[3]) + [4]*exp(-(x-1.0)/[5]))", 0., 30.);

	func->SetParameter(1, 1.4);
	func->SetParameter(3, 3.8);
	func->SetParameter(5, 45.);

	func->SetParameter(0, 1387.); // 1387
	func->SetParameter(2, 269.); // 269
	func->SetParameter(4, 19.); // 19
/*
	TCanvas* canv1 = new TCanvas("canv1");
	func->Draw();
	gPad->SetGrid(1, 1);
	gPad->SetLogy();
	func->GetYaxis()->SetRangeUser(20, 20000);
*/
	TImage* img1 = TImage::Open("Tau_fluoro_WLS_JLU.jpg");
	TCanvas* canv2 = new TCanvas("canv2");
	img1->Draw("x");

	TPad *p = new TPad("p","p",0,0,1,1);
	p->SetFillStyle(4000);
	p->SetFrameFillStyle(4000);
	p->Draw("");
	p->cd();

	func->Draw();
	gPad->SetGrid(1, 1);
	gPad->SetLogy();
	func->SetLineColor(kGreen);
	func->SetLineWidth(4);
	func->GetYaxis()->SetRangeUser(20, 20000);
}