#define INDEX 11

void fitAll(TString filename="scans/thrScans_from_pairs.root")
{
	TH1D* thrScan[64];

	// Read threshold scans from the external file
	TFile* inputFile = new TFile(filename, "READ");
	if (inputFile->IsZombie()) {
		cerr << "Error opening file " << filename.Data() << ". Aborting." << endl;
		return;
	}

	for (UInt_t i=0; i<64; i++) {
		TString histoName;
		histoName.Form("scanCurve_ch%d", i);
		thrScan[i] = (TH1D*)inputFile->Get(histoName);
		if (thrScan[i] == NULL) {
			cerr << "Error. Histogram " << histoName << " not found in " << filename.Data() << ". Aborting." << endl;
			return;
		}
	}

	// Draw the scan
	TCanvas* canv = new TCanvas("canv", "canv", 1364, 796);
	canv->Divide(2,2); canv->cd(2);
	gPad->SetGrid(1, 1);
	thrScan[INDEX]->SetStats(kFALSE);
	thrScan[INDEX]->Draw();


	// Fit the scan
	TF1* polynom = new TF1("polynom", "[0] + [1]*x + [2]*x**2 + [3]*x**3 + [4]*x**4 + [5]*x**5 + [6]*x**6 + [7]*x**7", -0x7f80, -0x7600);

	//FIXME - tune it!
	TVirtualFitter::SetMaxIterations(4000);
	cout << "Default precision: " << TVirtualFitter::GetPrecision() << endl;
	TVirtualFitter::SetPrecision(1e-2);
	cout << "Forced precision: " << TVirtualFitter::GetPrecision() << endl;

	thrScan[INDEX]->Fit(polynom, "", "", -0x7f80, -0x7600);


	// Find and draw the derivative
	//TCanvas* canv2 = new TCanvas("canv2", "canv2", 1364, 796);
	canv->cd(4);
	gPad->SetGrid(1, 1);

	// Analytical function - derivative of the fitting curve
	TF1* myDeriv = new TF1("myDeriv", "-([0] + 2*[1]*x + 3*[2]*x**2 + 4*[3]*x**3 + 5*[4]*x**4 + 6*[5]*x**5 + 7*[6]*x**6)", -0x7f80, -0x7600);
	myDeriv->SetParameters(polynom->GetParameter(1),
							polynom->GetParameter(2),
							polynom->GetParameter(3),
							polynom->GetParameter(4),
							polynom->GetParameter(5),
							polynom->GetParameter(6),
							polynom->GetParameter(7));

	myDeriv->SetTitle("Derivative of the threshold scan fit");
	myDeriv->SetLineWidth(2);
	myDeriv->SetLineColor(kBlack);
	myDeriv->GetXaxis()->SetTitle("Threshold");
	myDeriv->Draw();
/*
	thrScan[INDEX]->GetXaxis()->SetRangeUser(-0x7f80 + 0x20*STARTPOINT, -0x7600);
	myDeriv->GetXaxis()->SetRangeUser(-0x7f80 + 0x20*STARTPOINT, -0x7600);
*/
/*
	TFile* outputFile = new TFile("fittedScans.root", "RECREATE");
	for (UInt_t i=0; i<64; i++) {
		thrScan[i]->SetStats(kFALSE);
		thrScan[i]->Write();
	}
*/
}
