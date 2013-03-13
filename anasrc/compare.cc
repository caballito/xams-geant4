/****************************************************
 * Read multiple event trees of G4 XAMS simulations *
 * to compare them.                                 *
 * Make histograms for interesting parameters and   *
 * save them in an output file.                     *
 *                       --  Rolf Schön, March 2013 *
 ****************************************************/

#include "TDirectoryFile.h"
#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include <iostream>
#include "TH1.h"
#include "TH2.h"
#include <sstream>
using namespace std ;

int makeSteplengthHists(const int, Int_t, TH1F*[], TTree*[]) ;
int makeEdprocHists(const int, Int_t, TH1D***, TTree*[]) ;

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
		
		// Make histograms.
		if ( makeSteplengthHists(nFiles, i, steplength, tree) )
			cout << "EE Something went wrong with the step length histograms."
				<< endl ;
		if ( makeEdprocHists(nFiles, i, depoproc, tree) )
			cout << "EE Something went wrong with the energy deposition process histograms." << endl ;
	}

	// Save all histograms.
	TString outName("output.root") ;
	TFile out(outName,"recreate") ;
	out.cd() ;
	for (Int_t i=0;i<nFiles;i++){
		steplength[i]->Write() ;
		for (Int_t j=0;j<2;j++)
			depoproc[i][j]->Write() ;
	}
	cout << "++ Histograms written into " << outName << endl ;
	out.Close() ;

	return 0 ;
}


int makeSteplengthHists(const int nFiles, Int_t i,
		TH1F* steplength[], TTree* tree[]){
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

	return 0 ;
}


int makeEdprocHists(const int nFiles, Int_t i,
		TH1D*** depoproc, TTree* tree[]){
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

	return 0 ;
}
