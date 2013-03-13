/************************************************
 * Collection of commands to read in histograms *
 * that are inside the ouput file of compare(). *
 *                    -- Rolf Schön, March 2013 *
 ************************************************/

//-------------------------------------------\
// Before executing don't forget to adjust:  |
// - the file name (f),                      |
// - the number of files (nbr),              |
// - the labels for the legend (tags[]),     |
// - and the header of the legend.           |
//-------------------------------------------/

{
	TFile f("output.root") ;

	const int np = 3 ;	// Number of parameters, e.g step length, deposited energy.
	const int nbr = 2 ;	// Number of compared files = number of histograms per parameter.
	Int_t j = 0 ;	// Counter to np.
	Int_t i = 0 ;	// Counter to nbr.

	TH1F* hist1[nbr] ;
	TH1D* hist2[nbr] ;
	TH1D* hist3[nbr] ;

	stringstream dummy[np][nbr] ;
	TString names[np][nbr] ;
	//TString tags[nbr] = {"1 #mum","10 #mum","100 #mum","1 mm"} ;
	//TString tags[nbr] = {"1000 keV","800 keV","600 keV","400 keV","200 keV","100 keV","80 keV"} ;
	TString tags[nbr] = {"400 keV","200 keV"} ;

	for (i=0; i<nbr; i++){
		dummy[0][i] << "steplength[" << i << "]" ;
		dummy[1][i] << "depoproc[" << i << "][0]" ;	// For gammas.
		dummy[2][i] << "depoproc[" << i << "][1]" ;	// For electrons.

		for (j=0; j<np; j++){
			names[j][i] = (dummy[j][i].str()).c_str() ;
		}

		hist1[i] = dynamic_cast<TH1F*>(f->Get(names[0][i])) ;
		hist2[i] = dynamic_cast<TH1D*>(f->Get(names[1][i])) ;
		hist3[i] = dynamic_cast<TH1D*>(f->Get(names[2][i])) ;
	}
	cout << "++ Found all the histograms." << endl ;

	// Initialise legends.
	TLegend* leg[np] ;
	for (j=0; j<np; j++){
		leg[j] = new TLegend(0.7,0.7,0.9,0.9) ;
		leg[j]->SetFillColor(0) ;
		//leg[j]->SetHeader("range cut") ;
		leg[j]->SetHeader("initial gamma energy") ;
	}

	for (i=0; i<nbr; i++){
		hist1[i]->SetFillColor(19-i) ;
		hist2[i]->SetFillColor(19-i) ;
		hist3[i]->SetFillColor(19-i) ;

		leg[0]->AddEntry(hist1[i],tags[i],"f") ;
		leg[1]->AddEntry(hist2[i],tags[i],"f") ;
		leg[2]->AddEntry(hist3[i],tags[i],"f") ;
	}
	cout << "++ Built legends for the " << np << " comparisons." << endl ;

	// Make separate canvases.
	TCanvas can1("can1"), can2("can2"), can3("can3") ;

	// Draw histograms.
	can1.cd() ;
	hist1[0]->Draw() ;
	for (i=1; i<nbr; i++)
		hist1[i]->Draw("same") ;
	leg[0]->Draw("same") ;

	can2.cd() ;
	hist2[0]->Draw() ;
	for (i=1; i<nbr; i++)
		hist2[i]->Draw("same") ;
	leg[1]->Draw("same") ;

	can3.cd() ;
	hist3[0]->Draw() ;
	for (i=1; i<nbr; i++)
		hist3[i]->Draw("same") ;
	leg[2]->Draw("same") ;

	cout << "++ Done. You can save the plots now :)" << endl ;
}
