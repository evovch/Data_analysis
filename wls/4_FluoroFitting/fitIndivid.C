void fitIndivid(TString inFilename="300ttj_take3/simulation_res_300ttj_sum.root",
	Bool_t fixedTaus = kFALSE,
	Double_t xStart = 4.,
	TString outputFilename = "canvas.eps")
{
	//Double_t xRange = 90.;

	Double_t taus[3] = {1.4, 3.8, 45};

	// -----------------------------------------------------------------

	// Open the input file with histograms
	TFile* inFile = new TFile(inFilename, "READ");
	if (inFile->IsZombie()) {
		cerr << "Error opening file " << inFilename.Data() << ". Aborting." << endl;
		return;
	}

	// Get the histograms from the input file

	TH1D* inputHisto1 = (TH1D*)inFile->Get("hTimeProfile_0");
	if (inputHisto1 == NULL) {
		cerr << "Input histogram not found. Aborting." << endl;
		return;
	}

	TH1D* inputHisto2 = (TH1D*)inFile->Get("hTimeProfile_1");
	if (inputHisto2 == NULL) {
		cerr << "Input histogram not found. Aborting." << endl;
		return;
	}

	TH1D* inputHisto3 = (TH1D*)inFile->Get("hTimeProfile_2");
	if (inputHisto3 == NULL) {
		cerr << "Input histogram not found. Aborting." << endl;
		return;
	}

	TH1D* inputHisto4 = (TH1D*)inFile->Get("hTimeProfile");
	if (inputHisto4 == NULL) {
		cerr << "Input histogram not found. Aborting." << endl;
		return;
	}

	TH1D* inputHisto5 = (TH1D*)inFile->Get("hMinusFirstInTime");
	if (inputHisto5 == NULL) {
		cerr << "Input histogram not found. Aborting." << endl;
		return;
	}

	// -----------------------------------------------------------------

	// Fit

	TString functionString;
	functionString = "[0]*exp(-x/[1])";

	TF1* fitFunc1 = new TF1("fitFunc1", functionString, xStart, 20.);
	fitFunc1->SetParameter(0, 1.);
	fitFunc1->SetParameter(1, 1.);

	TF1* fitFunc2 = new TF1("fitFunc2", functionString, xStart, 50.);
	fitFunc2->SetParameter(0, 1.);
	fitFunc2->SetParameter(1, 1.);

	TF1* fitFunc3 = new TF1("fitFunc3", functionString, xStart, 90.);
	fitFunc3->SetParameter(0, 1.);
	fitFunc3->SetParameter(1, 1.);

	TString functionString2;
	UInt_t tau1index=1, tau2index=3, tau3index=5;
	UInt_t amp1index=0, amp2index=2, amp3index=4;
	functionString2 = "[0]*exp(-x/[1]) + [2]*exp(-x/[3]) + [4]*exp(-x/[5])";

	if (fixedTaus) {
		functionString2 = "[0]*exp(-x/1.4) + [1]*exp(-x/3.8) + [2]*exp(-x/45.)";
		amp1index=0;
		amp2index=1;
		amp3index=2;
	}

	cout << functionString2.Data() << endl;

	TF1* fitFunc4 = new TF1("fitFunc4", functionString2, xStart, 90.);
	fitFunc4->SetParameter(amp1index, 200000000.);
	fitFunc4->SetParameter(amp2index, 14000000.);
	fitFunc4->SetParameter(amp3index, 25000.);
	if (!fixedTaus) {
		fitFunc4->SetParameter(tau1index, 1.);
		fitFunc4->SetParameter(tau2index, 4.);
		fitFunc4->SetParameter(tau3index, 40.);
	}

	TF1* fitFunc5 = new TF1("fitFunc5", functionString2, xStart, 90.);
	fitFunc5->SetParameter(amp1index, 200000000.);
	fitFunc5->SetParameter(amp2index, 14000000.);
	fitFunc5->SetParameter(amp3index, 25000.);
	if (!fixedTaus) {
		fitFunc5->SetParameter(tau1index, 1.);
		fitFunc5->SetParameter(tau2index, 4.);
		fitFunc5->SetParameter(tau3index, 40.);
	}

	// -----------------------------------------------------------------

	// Draw results

	TCanvas* canv = new TCanvas("canv", "canv", 100, 100, 1400, 800);

	canv->Divide(3, 2);

	canv->cd(1);
	inputHisto1->Draw();
	inputHisto1->Fit(fitFunc1, "R");
	gPad->SetGrid(1,1);
	gPad->SetLogy();

	canv->cd(2);
	inputHisto2->Draw();
	inputHisto2->Fit(fitFunc2, "R");
	gPad->SetGrid(1,1);
	gPad->SetLogy();

	canv->cd(3);
	inputHisto3->Draw();
	inputHisto3->Fit(fitFunc3, "R");
	gPad->SetGrid(1,1);
	gPad->SetLogy();

	canv->cd(4);
	inputHisto4->Draw();
	inputHisto4->Fit(fitFunc4, "R");
	gPad->SetGrid(1,1);
	gPad->SetLogy();

	canv->cd(5);
	inputHisto5->Draw();
	inputHisto5->Fit(fitFunc5, "R");
	gPad->SetGrid(1,1);
	gPad->SetLogy();

	canv->SaveAs(outputFilename);

	// -----------------------------------------------------------------

	cout << endl;
	if (fixedTaus) cout << "Fitting with time constants fixed." << endl;
	else cout << "Fitting with all six parameters." << endl;
	cout << endl;

	cout << "Input file: " << inFilename << endl;
	cout << endl;

	cout << std::fixed << std::setprecision(8);
	cout << "Component 1" << endl;
	cout << fitFunc1->GetParameter(1) << ":\t" << fitFunc1->GetParameter(0) << endl << endl;
	cout << "Component 2" << endl;
	cout << fitFunc2->GetParameter(1) << ":\t" << fitFunc2->GetParameter(0) << endl << endl;
	cout << "Component 3" << endl;
	cout << fitFunc3->GetParameter(1) << ":\t" << fitFunc3->GetParameter(0) << endl << endl;

	cout << "Full time profile" << endl;

	if (fixedTaus) cout << taus[0] << ":\t";
	else cout << fitFunc4->GetParameter(tau1index) << ":\t";
	cout << fitFunc4->GetParameter(amp1index) << endl;

	if (fixedTaus) cout << taus[1] << ":\t";
	else cout << fitFunc4->GetParameter(tau2index) << ":\t";
	cout << fitFunc4->GetParameter(amp2index) << endl;

	if (fixedTaus) cout << taus[2] << ":\t";
	else cout << fitFunc4->GetParameter(tau3index) << ":\t";
	cout << fitFunc4->GetParameter(amp3index) << endl << endl;

	cout << "Minus first in time" << endl;

	if (fixedTaus) cout << taus[0] << ":\t";
	else cout << fitFunc5->GetParameter(tau1index) << ":\t";
	cout << fitFunc5->GetParameter(amp1index) << endl;

	if (fixedTaus) cout << taus[1] << ":\t";
	else cout << fitFunc5->GetParameter(tau2index) << ":\t";
	cout << fitFunc5->GetParameter(amp2index) << endl;

	if (fixedTaus) cout << taus[2] << ":\t";
	else cout << fitFunc5->GetParameter(tau3index) << ":\t";
	cout << fitFunc5->GetParameter(amp3index) << endl << endl;
}