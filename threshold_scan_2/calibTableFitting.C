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

void calibTableFitting(TString p_filename="calibration_files/calib_sum.root")
{
	TFile* inputFile = new TFile(p_filename, "READ");

	const UInt_t NUMTDCs = 1;
	const UInt_t NUMCHs = 33;

	TH1D* outputHisto[NUMTDCs][(NUMCHs-1)];
//	TH2D* ABmap = new TH2D("ABmap", "ABmap", 5*(Xmax-Xmin), Xmin, Xmax, 5*(Ymax-Ymin), Ymin, Ymax);
//	TH1D* Ahisto = new TH1D("Ahisto", "Ahisto", 100, 0., 100.);
//	TH1D* Bhisto = new TH1D("Bhisto", "Bhisto", 200, 400., 600.);

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

			//ABmap->Fill(A[tdc][ch-1], B[tdc][ch-1]);
			//Ahisto->Fill(A[tdc][ch-1]);
			//Bhisto->Fill(B[tdc][ch-1]);

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

/*
//	TCanvas* canvGraph = new TCanvas("canvGraph", "canvGraph", 1364, 796);
//	TGraph* ABgraph = new TGraph();

//	gStyle->SetMarkerSize(1);
//	gStyle->SetMarkerStyle(20);
//	gStyle->SetMarkerColor(kRed);

	for (UInt_t tdc=0; tdc<NUMTDCs; tdc++) {
		for (UInt_t ch=1; ch<NUMCHs; ch++) {
			ABgraph->SetPoint(tdc*NUMCHs + ch-1, A[tdc][ch-1], B[tdc][ch-1]);
		}
	}
	ABgraph->Draw("A*");
	gPad->SetGrid(1, 1);
	ABgraph->GetXaxis()->SetRangeUser(20., 34.);
	ABgraph->GetYaxis()->SetRangeUser(420., 560.);
	ABgraph->SetMarkerStyle(kFullDotMedium);
	ABgraph->GetXaxis()->SetTitle("Fine time counter value");
	ABgraph->GetYaxis()->SetTitle("Fine time counter value");

//	canvGraph->SaveAs("ABmap.eps");
//	canvGraph->SaveAs("ABmap.png");
*/

outside:
/*
	TCanvas* canv2 = new TCanvas("ABmap", "ABmap", 1364, 796);
	ABmap->Draw("COLZ"); // COLZ
	ABmap->GetXaxis()->SetTitle("Fine time counter value");
	ABmap->GetYaxis()->SetTitle("Fine time counter value");
	gPad->SetGrid(1, 1);
	gPad->Update();

	TPaveStats *ps1 = (TPaveStats*)ABmap->GetListOfFunctions()->FindObject("stats");
	ps1->SetX1NDC(0.60); ps1->SetX2NDC(0.85);
	gPad->Modified();

	//canv2->SaveAs("ABmap2.eps");
	//canv2->SaveAs("ABmap2.png");

	//inputFile->Close();
*/
	TFile* outputFile = new TFile("globalCalibFitting.root", "RECREATE");

//	ABmap->Write();
//	Ahisto->Write();
//	Bhisto->Write();

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
