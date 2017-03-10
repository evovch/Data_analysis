void drawToT(UInt_t p_ch=2,
			 TString p_filename="analysis_info_te15134132956.hld.root",
			 TString p_dirNoCalib="results_nocalib",
			 TString p_dirCalibPseudoGlobal="results_calib_global_pseudocalib",
			 TString p_dirCalibPseudoIndiv="results_calib_individual_pseudocalib",
			 TString p_dirCalibReal="results_calib_real")
{
	TString filename[4];

	filename[0].Form("%s/%s", p_dirNoCalib.Data(), p_filename.Data());
	filename[1].Form("%s/%s", p_dirCalibPseudoGlobal.Data(), p_filename.Data());
	filename[2].Form("%s/%s", p_dirCalibPseudoIndiv.Data(), p_filename.Data());
	filename[3].Form("%s/%s", p_dirCalibReal.Data(), p_filename.Data());

	TFile* inFile[4];
	for (UInt_t i=0; i<4; i++) {
		inFile[i] = new TFile(filename[i], "READ");
		if (inFile[i]->IsZombie()) {
			cerr << "Error opening file " << filename[i].Data() << ". Aborting." << endl;
			return;
		}
	}

	TString histoName;
	histoName.Form("ToT_ch%d", p_ch);

	TH1D* histo[4];
	for (UInt_t i=0; i<4; i++) {
		histo[i] = (TH1D*)inFile[i]->Get(histoName);
		if (histo[i] == NULL) {
			cerr << "Error. Histogram " << histoName << " not found in " << filename[i].Data() << ". Aborting." << endl;
			return;
		}
	}

	TString canvName;
	canvName.Form("canv_ch%d", p_ch);
	TCanvas* canv = new TCanvas(canvName, canvName, 1364, 796);

	canv->Divide(2, 2);

	gStyle->SetOptStat(1111);

	for (UInt_t i=0; i<4; i++) {
		canv->cd(i+1);
		histo[i]->Draw();
		histo[i]->Rebin(4);
		histo[i]->SetLineWidth(3);

		if (p_ch==4 || p_ch==5) {
			histo[i]->GetXaxis()->SetRangeUser(10.7, 11.7);
		} else if (p_ch==6) {
			histo[i]->GetXaxis()->SetRangeUser(8.4, 9.4);
		} else if (p_ch==8 || p_ch==9 || p_ch==10) {
			histo[i]->GetXaxis()->SetRangeUser(10.5, 11.5);
		} else if (p_ch==14 || p_ch==15 || p_ch==11) {
			histo[i]->GetXaxis()->SetRangeUser(10.2, 11.2);
		} else if (p_ch==16) {
			histo[i]->GetXaxis()->SetRangeUser(9., 10.);
		} else {
			histo[i]->GetXaxis()->SetRangeUser(10., 11.);
		}

		histo[i]->GetYaxis()->SetRangeUser(100, 1000000.);
		histo[i]->GetXaxis()->SetTitle("ns ");
		histo[i]->GetXaxis()->SetTitleOffset(0.8);
		histo[i]->GetXaxis()->SetTitleSize(0.06);
		histo[i]->GetXaxis()->SetLabelSize(0.05);
		histo[i]->GetYaxis()->SetTitle("Entries");
		histo[i]->GetYaxis()->SetTitleOffset(0.7);
		histo[i]->GetYaxis()->SetTitleSize(0.07);
		histo[i]->GetYaxis()->SetLabelSize(0.06);
		gPad->SetGrid(1, 1);
		gPad->SetLogy();

		gPad->Update();
		TPaveStats *ps1 = (TPaveStats*)histo[i]->GetListOfFunctions()->FindObject("stats");
		if (ps1 == NULL) {
			cerr << "Stat box not found." << endl;
		} else {
			ps1->SetX1NDC(0.75); ps1->SetX2NDC(0.99);
			ps1->SetY1NDC(0.57); ps1->SetY2NDC(0.95);
			//ps1->SetTextColor(kBlue);
			gPad->Modified();
		}
	}

	TString histoTitle;

/*	histoTitle.Form("ToT channel %d, no fine time calibration", p_ch);
	histo[0]->SetTitle(histoTitle);
	histoTitle.Form("ToT channel %d, global pseudo fine time calibration", p_ch);
	histo[1]->SetTitle(histoTitle);
	histoTitle.Form("ToT channel %d, individual pseudo fine time calibration", p_ch);
	histo[2]->SetTitle(histoTitle);
	histoTitle.Form("ToT channel %d, real fine time calibration", p_ch);
	histo[3]->SetTitle(histoTitle);
*/

	histo[0]->SetTitle("      a");
	histo[1]->SetTitle("      b");
	histo[2]->SetTitle("      c");
	histo[3]->SetTitle("      d");
	
	TString outFilename;
	outFilename.Form("pictures_ToT/ToT_ch%d.png", p_ch);
	canv->SaveAs(outFilename);
	outFilename.Form("pictures_ToT/ToT_ch%d.eps", p_ch);
	canv->SaveAs(outFilename);

}
