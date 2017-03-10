/*********************************
From the experimental data fitting
4939.64	-	1.4 ns
839.54	-	3.8 ns
15.67	-	45 ns
*********************************/

#include <iostream>
using std::cout;
using std::endl;

#include <TString.h>
#include <TRandom3.h>
#include <TH1D.h>
#include <TF1.h>
#include <TFile.h>

void gen(TString suffix, UInt_t nEvents);

int main (int argc, char** argv)
{
	TString suffix="";
	UInt_t nEvents=100000;

	switch (argc) {
	case 1:
		break;
	case 2:
		suffix = TString(argv[1]);
		break;
	case 3:
		suffix = TString(argv[1]);
		nEvents = atoi(argv[2]);
		break;
	default:
		cout << "Wrong number of arguments." << endl;
		return 1;
	}

	gen(suffix, nEvents);

	return 0;
}

void gen(TString suffix /*=""*/, UInt_t nEvents /*=100000*/)
{
	cout << "Simulating " << nEvents << " events." << endl;

	Double_t taus[3] = {1.4, 3.8, 45};
	Double_t amps[3] = {4939.64, 839.54, 15.67};
	Double_t ampSum = amps[0] + amps[1] + amps[2];

	// Mean number of hits per event (per ring)
	UInt_t Nmean = 18;

	// Simulated time range, ns
	Double_t xRange = 100.;

	// Define bin size, ns
	Double_t binSize = 0.1;

	// Transition time jitter, ns
	Double_t ttj = 0.3;

	// Random number generator
	TRandom3* rndGen = new TRandom3();
	rndGen->SetSeed();

	// Create a string for the name of the function and fill it
	TString funcName;
	funcName.Form("expFunc");

	// Create an analytical function
	TF1* expFunc = new TF1(funcName, "exp(-x/[0])", 0., xRange);

	TH1D* hNelectrons[3];
	TH1D* hTimeProfile[3];
	TH1D* hTimeProfileFull;
	TH1D* hMinusFirstInTime;

	TString histoName;
	TString histoTitle;
	for (UInt_t iComponent=0; iComponent<3; iComponent++)
	{
		histoName.Form("hNelectrons_%d", iComponent);
		histoTitle.Form("Number of generated photoelectrons for component %d;Electrons;Entries", iComponent);
		hNelectrons[iComponent] = new TH1D(histoName, histoTitle, 50, 0., 50.);

		histoName.Form("hTimeProfile_%d", iComponent);
		histoTitle.Form("Time profile for tau %f;ns;1/%0.2f", taus[iComponent], binSize);
		hTimeProfile[iComponent] = new TH1D(histoName, histoTitle, (UInt_t)(xRange/binSize), 0., xRange);
	}

	histoTitle.Form("Time profile full;ns;1/%0.2f", binSize);
	hTimeProfileFull = new TH1D("hTimeProfile", histoTitle, (UInt_t)(xRange/binSize), 0., xRange);

	histoTitle.Form("Minus first in time;ns;1/%0.2f", binSize);
	hMinusFirstInTime = new TH1D("hMinusFirstInTime", histoTitle, (UInt_t)(xRange/binSize), 0., xRange);

	Double_t times[1000];
	UInt_t Npelectrons[3];

	// Loop over the events
	for (UInt_t iEvent=0; iEvent<nEvents; iEvent++) {

		if (iEvent%1000 == 0)
			cout << "Event " << iEvent << endl;

		for (UInt_t iComponent=0; iComponent<3; iComponent++)
		{
			// Get the number of photoelectrons of a given component in this event from the Poisson distribution
			Npelectrons[iComponent] = rndGen->Poisson(Nmean*amps[iComponent]/ampSum);

			// Fill the histogram - number of photoelectrons of a given component per event
			hNelectrons[iComponent]->Fill(Npelectrons[iComponent]);
		}

		// Get the total number of photoelectrons in the event
		UInt_t Nsum = 0;
		for (UInt_t iComponent=0; iComponent<3; iComponent++) { Nsum += Npelectrons[iComponent]; }

		//cout << Npelectrons[0] << "+" << Npelectrons[1] << "+" << Npelectrons[2] << "=" << Nsum << endl;

		// Array of times generated for this event
		//Double_t* times = new Double_t[Nsum];

		UInt_t counter=0;
		for (UInt_t iComponent=0; iComponent<3; iComponent++) {
			// Loop over the photoelectrons
			for (UInt_t iPelectron=0; iPelectron<Npelectrons[iComponent]; iPelectron++) {
				// Get actual decay time for this photoelectron from the user-created analytical function
				expFunc->SetParameter(0, taus[iComponent]);
				Double_t time = expFunc->GetRandom();

				// Transition time jitter
				Double_t ttjSpread = rndGen->Gaus(ttj);

				// Save the generated time
				times[counter++] = time+ttjSpread;

				// And put it into the histogram
				hTimeProfile[iComponent]->Fill(time+ttjSpread);
				hTimeProfileFull->Fill(time+ttjSpread);
			}
		}

		if (Nsum != counter) cout << "Error" << endl;

		// Find the first hit in time in this event
		UInt_t iFastestPelectron=0;
		for (UInt_t iPelectron=1; iPelectron<Nsum; iPelectron++) {
			if (times[iPelectron] < times[iFastestPelectron]) iFastestPelectron = iPelectron;
		}

		// Fill the histogram
		for (UInt_t iPelectron=0; iPelectron<Nsum; iPelectron++) {
			// Skip the first in time electron
			if (iPelectron != iFastestPelectron) {
				hMinusFirstInTime->Fill(times[iPelectron]-times[iFastestPelectron]);
			}
		}

		//delete [] times;

	}

	// Create the output file
	TString outFilename;
	outFilename.Form("simulation_res_%s.root", suffix.Data());
	TFile outFile(outFilename, "RECREATE");

	// Write histograms into the output file
	for (UInt_t iComponent=0; iComponent<3; iComponent++) {
		hTimeProfile[iComponent]->Write();
		hNelectrons[iComponent]->Write();
	}
	hTimeProfileFull->Write();
	hMinusFirstInTime->Write();

	// Close the file?
	outFile.Close();

	cout << "Done." << endl;
}
