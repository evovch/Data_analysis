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

	gStyle->SetOptStat(111111);

	for (UInt_t i=0; i<4; i++) {
		canv->cd(i+1);
		histo[i]->Draw();
		histo[i]->Rebin(4);
		histo[i]->SetLineWidth(2);
		//histo[i]->GetXaxis()->SetRangeUser(10., 11.);
		histo[i]->GetXaxis()->SetTitle("ns");
		histo[i]->GetYaxis()->SetTitle("Entries");
		gPad->SetGrid(1, 1);
		gPad->SetLogy();
	}

	TString histoTitle;
	histoTitle.Form("ToT channel %d, no fine time calibration", p_ch);
	histo[0]->SetTitle(histoTitle);
	histoTitle.Form("ToT channel %d, global pseudo fine time calibration", p_ch);
	histo[1]->SetTitle(histoTitle);
	histoTitle.Form("ToT channel %d, individual pseudo fine time calibration", p_ch);
	histo[2]->SetTitle(histoTitle);
	histoTitle.Form("ToT channel %d, real fine time calibration", p_ch);
	histo[3]->SetTitle(histoTitle);

	TString outFilename;
	outFilename.Form("pictures_ToT/ToT_noLog_ch%d.png", p_ch);
	canv->SaveAs(outFilename);
	outFilename.Form("pictures_ToT/ToT_noLog_ch%d.eps", p_ch);
	canv->SaveAs(outFilename);

}
