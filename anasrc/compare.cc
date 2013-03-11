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

int makeSteplengthHists(const int, Int_t, TH1F*[], TTree*[], TLegend*) ;

int compare(Int_t nbrFiles, TString fName[]){
	const int nFiles = nbrFiles ;
	TFile* f[nFiles] ;
	TDirectoryFile* dir[nFiles] ;
	TTree* tree[nFiles] ;
	//
	TH1F* steplength[nFiles] ;
	TLegend* leg = new TLegend(0.7,0.6,0.9,0.9) ;
	leg->SetFillColor(0) ;
	
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
		
		if ( makeSteplengthHists(nFiles,i,steplength,tree,leg) )
			cout << "EE Something's wrong with making the step length histograms."
				<< endl ;
	}

	steplength[0]->Draw() ;
	for (Int_t i=1; i<nFiles; i++)
		steplength[i]->Draw("same") ;
	leg->Draw("same") ;

	//delete f ; delete dir ; delete tree ; delete steplength ;

	return 0 ;
}

int makeSteplengthHists(const int nFiles, Int_t i, TH1F* steplength[],
		TTree* tree[], TLegend* leg){
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
		"step length;step length (mm);entries",500,0,500) ;
	tree[i]->Draw(drawTarget[i],"type.data()==\"gamma\" ") ;
	cout << "++ Drawing " << histName[i] << " with " 
		<< steplength[i]->GetEntries() << " entries." << endl ;

	steplength[i]->SetFillColor(19-i) ;
	leg->AddEntry(steplength[i],histName[i],"f") ;

	return 0 ;
}
