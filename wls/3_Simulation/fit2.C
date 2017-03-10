void fit2(TString inFilename="300ttj_take3/simulation_res_300ttj_sum.root", Bool_t fixedTaus = kFALSE, TString outputFilename = "canvas.eps")
{
	if (fixedTaus) cout << "Fitting with time constants fixed." << endl;
	else cout << "Fitting with all six parameters." << endl;

	// -------------------------------------------------------------------

	Double_t xStart = 4.;
	Double_t xRange = 90.;

	Double_t taus[3] = {1.4, 3.8, 45};
	Double_t amps[3] = {4939.64, 839.54, 15.67};

	// -------------------------------------------------------------------

	// Create a string for the analytical function

	TString functionString;

	UInt_t tau1index=1, tau2index=3, tau3index=5;
	UInt_t amp1index=0, amp2index=2, amp3index=4;
	functionString = "[0]*exp(-(x-1.0)/[1]) + [2]*exp(-(x-1.0)/[3]) + [4]*exp(-(x-1.0)/[5])";

	if (fixedTaus) {
		functionString = "[0]*exp(-(x-1.0)/1.4) + [1]*exp(-(x-1.0)/3.8) + [2]*exp(-(x-1.0)/45.)";
		amp1index=0;
		amp2index=1;
		amp3index=2;
	}

	cout << functionString.Data() << endl;

	// -------------------------------------------------------------------

	// Create an analytical function, set initial parameters and set fitting precision

	TF1* fitFunc = new TF1("fitFunc", functionString, xStart, xRange);

	if (!fixedTaus) {
		fitFunc->SetParameter(tau1index, 1.3);
		fitFunc->SetParameter(tau2index, 3.9);
		fitFunc->SetParameter(tau3index, 50.);

		fitFunc->SetParameter(amp1index, 320975018.952835);
		fitFunc->SetParameter(amp2index, 14925232.343868);
		fitFunc->SetParameter(amp3index, 24063.101131);
	}
/*
	TVirtualFitter::SetMaxIterations(100000);
	cout << "Default precision: " << TVirtualFitter::GetPrecision() << endl;
	TVirtualFitter::SetPrecision(1e-4);
	cout << "Forced precision: " << TVirtualFitter::GetPrecision() << endl;
*/
	// -------------------------------------------------------------------

	// Create a canvas
	TCanvas* canv = new TCanvas("canv", "canv", 100, 100, 1400, 800);

	// Open the input file with a histogram
	TFile* inFile = new TFile(inFilename, "READ");
	if (inFile->IsZombie()) {
		cerr << "Error opening file " << inFilename.Data() << ". Aborting." << endl;
		return;
	}

	// Get ths histogram from the input file
	TH1D* inputHisto = (TH1D*)inFile->Get("hMinusFirstInTime"); // hTimeProfile // hMinusFirstInTime
	if (inputHisto == NULL) {
		cerr << "Input histogram not found. Aborting." << endl;
		return;
	}

	//inputHisto->Scale(0.01);
	inputHisto->Draw();
	inputHisto->Fit(fitFunc, "R");
	fitFunc->Draw("sames");
	gPad->SetGrid(1,1);
	gPad->SetLogy();

	// Print the fitting result

	Double_t resSum = fitFunc->GetParameter(0) + fitFunc->GetParameter(1) + fitFunc->GetParameter(2);

	cout << "Fitting result:-----------------------------------------------------" << endl;

	if (fixedTaus) {

		cout << std::fixed;
		cout << std::setprecision(6) << taus[0] << ":\t" << fitFunc->GetParameter(amp1index) << endl;
		cout << std::setprecision(6) << taus[1] << ":\t" << fitFunc->GetParameter(amp2index) << endl;
		cout << std::setprecision(6) << taus[2] << ":\t" << fitFunc->GetParameter(amp3index) << endl << endl;

		cout << "Ratio2:--------------------------------------------------------------" << endl;
		cout << std::fixed << std::setprecision(8);
		cout << taus[0] << ":\t" << 15.67*fitFunc->GetParameter(amp1index)/fitFunc->GetParameter(amp3index) << endl;
		cout << taus[1] << ":\t" << 15.67*fitFunc->GetParameter(amp2index)/fitFunc->GetParameter(amp3index) << endl;
		cout << taus[2] << ":\t" << 15.67*fitFunc->GetParameter(amp3index)/fitFunc->GetParameter(amp3index) << endl;
		cout << endl;

		cout << "Ratio:--------------------------------------------------------------" << endl;
		cout << std::fixed << std::setprecision(8);
		cout << taus[0] << ":\t" << fitFunc->GetParameter(amp1index)/resSum << endl;
		cout << taus[1] << ":\t" << fitFunc->GetParameter(amp2index)/resSum << endl;
		cout << taus[2] << ":\t" << fitFunc->GetParameter(amp3index)/resSum << endl;
		cout << endl;

	} else {

		cout << std::fixed;
		cout << std::setprecision(6) << fitFunc->GetParameter(tau1index) << ":\t " << fitFunc->GetParameter(amp1index) << endl;
		cout << std::setprecision(6) << fitFunc->GetParameter(tau2index) << ":\t" << fitFunc->GetParameter(amp2index) << endl;
		cout << std::setprecision(6) << fitFunc->GetParameter(tau3index) << ":\t" << fitFunc->GetParameter(amp3index) << endl << endl;

		cout << "Ratio:--------------------------------------------------------------" << endl;
		cout << std::fixed << std::setprecision(8);
		cout << taus[0] << ":\t" << fitFunc->GetParameter(amp1index)/resSum << endl;
		cout << taus[1] << ":\t" << fitFunc->GetParameter(amp2index)/resSum << endl;
		cout << taus[2] << ":\t" << fitFunc->GetParameter(amp3index)/resSum << endl;
		cout << endl;

	}

	canv->SaveAs(outputFilename);

/*
	// Just create and draw a function with given parameters
	// This works only for 4 components
	TF1* fitFunc2 = new TF1("fitFunc2", functionString, 0., xRange);
	fitFunc2->SetParameters(10., 10., 20., 5., 40., 2.5, 80., 1.25);
	TCanvas* canv2 = new TCanvas("canv2", "canv2", 100, 100, 1400, 800);
	fitFunc2->Draw();
	gPad->SetGrid(1,1);
	gPad->SetLogy();
*/
}
