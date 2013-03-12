/****************************************************
 * Read multiple event trees of G4 XAMS simulations *
 * to compare them.                                 *
 *                       --  Rolf Schön, March 2013 *
 ****************************************************/

#include "TDirectoryFile.h"
#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TEntryList.h"
#include <iostream>
#include "TH1.h"
#include "TH2.h"
#include "TF1.h"
#include "TMath.h"
#include "TLine.h"
#include "TCanvas.h"
#include "TLegend.h"
#include <sstream>
using namespace std ;

int makeSteplengthHists(const int, Int_t, TH1F*[], TTree*[], TLegend*[]) ;
int makeEdprocHists(const int, Int_t, TH1D***, TTree*[], TLegend*[]) ;

int compare(Int_t nbrFiles, TString fName[]){
	// Navigation to the trees.
	const int nFiles = nbrFiles ;
	TFile* f[nFiles] ;
	TDirectoryFile* dir[nFiles] ;
	TTree* tree[nFiles] ;
	// Histograms for the parameters to compare.
	TH1F* steplength[nFiles] ;
	TH1D*** depoproc ;
	depoproc = new TH1D**[nFiles] ;	// For e- and gamma.
	for (Int_t i=0; i<nFiles; i++)
		depoproc[i] = new TH1D*[2] ;
	//
	TLegend* leg[3] ;	// One for every histogram.
	leg[0] = new TLegend(0.7,0.6,0.9,0.9) ;
	leg[0]->SetFillColor(0) ;
	leg[1] = new TLegend(0.7,0.6,0.9,0.9) ;
	leg[1]->SetFillColor(0) ;
	leg[2] = new TLegend(0.7,0.6,0.9,0.9) ;
	leg[2]->SetFillColor(0) ;

	// For later use.
	TString outName("output.root") ;
	TFile out(outName,"recreate") ;
	TCanvas can1("can1") ;
	can1.cd() ;

	for (Int_t i=0; i<nFiles; i++){
		// Read in the files.
		f[i] = new TFile(fName[i]) ;
		if ( f[i]->IsOpen() ) {
	    cout << "++ Reading " << fName[i] << "..." << endl ; 
  	}
		else return 0 ;

		// Read in trees.
		dir[i] = dynamic_cast<TDirectoryFile*>(f[i]->Get("events")) ;
		dir[i]->cd() ;
		tree[i] = dynamic_cast<TTree*>(dir[i]->Get("events")) ;
		
		if ( makeSteplengthHists(nFiles, i, steplength, tree, leg) )
			cout << "EE Something went wrong with the step length histograms."
				<< endl ;

		if ( makeEdprocHists(nFiles, i, depoproc, tree, leg) )
			cout << "EE Something went wrong with the energy deposition process histograms." << endl ;
	}

	steplength[0]->Draw() ;
	for (Int_t i=1; i<nFiles; i++)
		steplength[i]->Draw("same") ;
	leg[0]->Draw("same") ;
	can1.Update() ;
	//can1.WaitPrimitive() ;
	//
	TCanvas can2("can2") ;
	can2.Divide(1,2) ;
	for (Int_t j=0;j<2;j++){
		can2.cd(j+1) ;
		depoproc[0][j]->Draw() ;
		for (Int_t i=1; i<nFiles; i++)
			depoproc[i][j]->Draw("same") ;
		leg[j+1]->Draw("same") ;
	}
	can2.Update() ;
	can2.WaitPrimitive() ;

	out.cd() ;
	can1.Write() ;
	can2.Write() ;
	cout << "Histograms written into " << outName << endl ;
	out.Close() ;

	can1.WaitPrimitive() ;
	can2.WaitPrimitive() ;
	return 0 ;
}

int makeSteplengthHists(const int nFiles, Int_t i, TH1F* steplength[],
		TTree* tree[], TLegend* leg[]){
	stringstream dummyName[nFiles] ;
	stringstream dummyTarget[nFiles] ;
	TString histName[nFiles] ;
	TString drawTarget[nFiles] ;
	
	// Create name strings.
	dummyName[i] << "steplength[" << i << "]" ;
	histName[i] = (dummyName[i].str()).c_str() ;
	dummyTarget[i] << "steplen>>" << histName[i] ;
	drawTarget[i] = (dummyTarget[i].str()).c_str() ;

	// Create and draw histograms.
	steplength[i] = new TH1F(histName[i],
		"step length of gammas;step length (mm);entries",500,0,500) ;
	tree[i]->Draw(drawTarget[i],"type.data()==\"gamma\" ") ;
	cout << "++ Drawing " << histName[i] << " with " 
		<< steplength[i]->GetEntries() << " entries." << endl ;

	steplength[i]->SetFillColor(19-i) ;
	leg[0]->AddEntry(steplength[i],histName[i],"f") ;

	return 0 ;
}

int makeEdprocHists(const int nFiles, Int_t i, TH1D*** depoproc,
		TTree* tree[], TLegend* leg[]){
	stringstream dummyName[nFiles][2] ;
	stringstream dummyTarget[nFiles][2] ;
	TString histName[nFiles][2] ;
	TString drawTarget[nFiles][2] ;
	
	// Create name strings.
	for (Int_t j=0;j<2;j++){
		dummyName[i][j] << "depoproc[" << i << "][" << j << "]" ;
		histName[i][j] = (dummyName[i][j].str()).c_str() ;
		dummyTarget[i][j] << "edproc>>" << histName[i][j] ;
		drawTarget[i][j] = (dummyTarget[i][j].str()).c_str() ;
	}

	// Create and draw histograms.
	// For gammas.
	depoproc[i][0] = new TH1D(histName[i][0],
		"gamma;energy deposition process;entries",5,0,5) ;
	tree[i]->Draw(drawTarget[i][0],"type.data()==\"gamma\" ") ;
	cout << "++ Drawing " << histName[i][0] << " with " 
		<< depoproc[i][0]->GetEntries() << " entries." << endl ;
	// For electrons.
	depoproc[i][1] = new TH1D(histName[i][1],
		"electron;energy deposition process;entries",5,0,5) ;
	tree[i]->Draw(drawTarget[i][1],"type.data()==\"e-\" ") ;
	//cout << "++ Drawing " << histName[i] << " with " 
	//	<< steplength[i]->GetEntries() << " entries." << endl ;

	depoproc[i][0]->SetFillColor(19-i) ;
	leg[1]->AddEntry(depoproc[i][0],histName[i][0],"f") ;
	depoproc[i][1]->SetFillColor(19-i) ;
	leg[2]->AddEntry(depoproc[i][1],histName[i][1],"f") ;

	return 0 ;
}
