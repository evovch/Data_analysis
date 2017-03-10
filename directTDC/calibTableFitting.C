// A - first boundary
// B - second boundary

Double_t Xmin = 22.;
Double_t Xmax = 32.;
Double_t Ymin = 440.;
Double_t Ymax = 560.;

Double_t fitF(Double_t* x, Double_t* par)
{
	if (x[0] < par[0]) {
		// First section - x < A
		return 0.;
	} else if (x[0] < par[1]) {
		// Second section - A < x < B
		return 5.*(x[0]-par[0])/(par[1]-par[0]);
	} else {
		// Third section - x > B
		return 5.;
	}
}

UInt_t IntegerToTDCid(UInt_t index)
{
    return (((index/4) + 1) << 4) + (index%4);
}

void calibTableFitting(TString p_filename="calib/calib_sum.root")
{
	TFile* inputFile = new TFile(p_filename, "READ");

	const UInt_t NUMTDCs = 1;
	const UInt_t NUMCHs = 33;

	TH1D* outputHisto[NUMTDCs][(NUMCHs-1)];
	TH2D* ABmap = new TH2D("ABmap", "ABmap", 20*(Xmax-Xmin), Xmin, Xmax, 5*(Ymax-Ymin), Ymin, Ymax);
	TH1D* Ahisto = new TH1D("Ahisto", "Ahisto", 100, 0., 100.);
	TH1D* Bhisto = new TH1D("Bhisto", "Bhisto", 200, 400., 600.);

	Double_t A[NUMTDCs][(NUMCHs-1)];
	Double_t B[NUMTDCs][(NUMCHs-1)];

	//NUMTDCs
	for (UInt_t tdc=0; tdc<NUMTDCs; tdc++) {
		UInt_t v_tdcId = IntegerToTDCid(tdc);

		TString tdcStr;
		tdcStr.Form("%04x", v_tdcId);

		//NUMCHs
		for (UInt_t ch=1; ch<NUMCHs; ch++) {

			TString histoName;
			histoName.Form("TDC%s/CalTable_%04x_%02d", tdcStr.Data(), v_tdcId, ch);

//			cout << histoName << endl;

			TH1D* curHisto;
			curHisto = (TH1D*)inputFile->Get(histoName);

			curHisto->Draw();

			TF1* fitFunc = new TF1("fitFunc", fitF, 0., 600., 2);
			fitFunc->SetParameters(30.,490.);
			curHisto->Fit("fitFunc");

			A[tdc][ch-1] = fitFunc->GetParameter(0);
			B[tdc][ch-1] = fitFunc->GetParameter(1);

			for (UInt_t k=0; k<ch; k++) {
				ABmap->Fill(A[tdc][ch-1], B[tdc][ch-1]);
			}

			Ahisto->Fill(A[tdc][ch-1]);
			Bhisto->Fill(B[tdc][ch-1]);

			histoName.Form("CalibTableMinusFit_TDC%s_CH%d", tdcStr.Data(), ch);
			outputHisto[tdc][ch-1] = new TH1D(histoName, histoName, 1024, 0., 1024.);

			for (UInt_t iBin=1; iBin<1024; iBin++) {
				outputHisto[tdc][ch-1]->SetBinContent(iBin, curHisto->GetBinContent(iBin) - fitFunc->Eval((Double_t)iBin-0.5));
			}

			/*if (A[tdc][ch-1] < 0.) {
				TCanvas* canv0 = new TCanvas("canv0", "canv0");
				curHisto->Draw();
				TCanvas* canv1 = new TCanvas("canv1", "canv1");
				fitFunc->Draw();
				goto outside;
			}*/

		} // end of loop over channels
	} // end of loop over TDCs

	TCanvas* canvGraph = new TCanvas("canvGraph", "canvGraph", 1364, 796);
	TGraph* ABgraphL = new TGraph();
	TGraph* ABgraphT = new TGraph();

	/*gStyle->SetMarkerSize(1);
	gStyle->SetMarkerStyle(20);
	gStyle->SetMarkerColor(kRed);*/

	UInt_t counterL=0;
	UInt_t counterT=0;

	for (UInt_t tdc=0; tdc<NUMTDCs; tdc++) {
		for (UInt_t ch=1; ch<NUMCHs; ch+=2) {
			ABgraphL->SetPoint(counterL++, A[tdc][ch-1], B[tdc][ch-1]);		// tdc*NUMCHs + ch-1
		}
		for (UInt_t ch=2; ch<NUMCHs; ch+=2) {
			ABgraphT->SetPoint(counterT++, A[tdc][ch-1], B[tdc][ch-1]);
		}
	}
	ABgraphL->Draw("A*");
	gPad->SetGrid(1, 1);
	ABgraphL->GetXaxis()->SetRangeUser(25.4, 29.3);
	ABgraphL->GetYaxis()->SetRangeUser(490., 520.);
	ABgraphL->GetXaxis()->SetTitle("Fine time counter value");
	ABgraphL->GetYaxis()->SetTitle("Fine time counter value");
	ABgraphL->SetMarkerStyle(kFullCircle); // kFullDotMedium
	ABgraphL->SetMarkerColor(kBlack);
	ABgraphL->SetMarkerSize(1);

	ABgraphT->Draw("*");
	ABgraphT->SetMarkerStyle(kFullCircle);
	ABgraphT->SetMarkerColor(kRed);
	ABgraphT->SetMarkerSize(1);

	TGraph* meanABgr = new TGraph();
	meanABgr->SetPoint(0, 27.3, 507.7);
	meanABgr->Draw("*");
	meanABgr->SetMarkerStyle(kFullSquare);
	meanABgr->SetMarkerColor(kRed);
	meanABgr->SetMarkerSize(3);

	UInt_t ch = 1;
	TGraph* usedABgrL = new TGraph();
	usedABgrL->SetPoint(0, A[0][ch-1], B[0][ch-1]);
	usedABgrL->Draw("*");
	usedABgrL->SetMarkerStyle(kFullTriangleUp);
	usedABgrL->SetMarkerColor(kBlack);
	usedABgrL->SetMarkerSize(3);

	ch = 2;
	TGraph* usedABgrT = new TGraph();
	usedABgrT->SetPoint(0, A[0][ch-1], B[0][ch-1]);
	usedABgrT->Draw("*");
	usedABgrT->SetMarkerStyle(kFullTriangleUp);
	usedABgrT->SetMarkerColor(kRed);
	usedABgrT->SetMarkerSize(3);

	canvGraph->SaveAs("ABmap.eps");
	canvGraph->SaveAs("ABmap.png");

	for (UInt_t ch=1; ch<NUMCHs; ch+=2) {
		cout << std::fixed;
		cout << std::setprecision(3);
		cout << "ch " << ch << ":\tLeading\t" << A[0][ch-1] << ",\t" << B[0][ch-1] << "\t\t";
		cout << "ch " << ch+1 << ":\tTrailing\t" << A[0][ch+1-1] << ",\t" << B[0][ch+1-1] << endl;
	}


outside:

	TCanvas* canv2 = new TCanvas("ABmap", "ABmap", 1364, 796);
	ABmap->Draw("TEXT"); // COLZ
	ABmap->GetXaxis()->SetTitle("Fine time counter value");
	ABmap->GetYaxis()->SetTitle("Fine time counter value");
	ABmap->GetXaxis()->SetRangeUser(25.4, 29.3);
	ABmap->GetYaxis()->SetRangeUser(490., 520.);
	gPad->SetGrid(1, 1);
	gPad->Update();

	TPaveStats *ps1 = (TPaveStats*)ABmap->GetListOfFunctions()->FindObject("stats");
	ps1->SetX1NDC(0.60); ps1->SetX2NDC(0.85);
	gPad->Modified();

	canv2->SaveAs("ABmap2.eps");
	canv2->SaveAs("ABmap2.png");

	//inputFile->Close();

	TFile* outputFile = new TFile("globalCalibFitting.root", "RECREATE");

	ABgraphL->Write();
	ABgraphT->Write();
	meanABgr->Write();

	ABmap->Write();
	Ahisto->Write();
	Bhisto->Write();

	for (UInt_t tdc=0; tdc<NUMTDCs; tdc++) {
		UInt_t v_tdcId = IntegerToTDCid(tdc);
        TString dirName;
        dirName.Form("TDC%04x", v_tdcId);
        gDirectory->mkdir(dirName);
        gDirectory->cd(dirName);
		for (UInt_t ch=1; ch<NUMCHs; ch++) {
			outputHisto[tdc][ch-1]->Write();
		}
		gDirectory->cd("..");
	}
	outputFile->Close();

}
