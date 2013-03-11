#include <G4HadronCaptureProcess.hh>

#include <G4ProcessManager.hh>
#include <G4ProcessVector.hh>
#include <G4ParticleDefinition.hh>
#include <G4ParticleWithCuts.hh>
#include <G4ParticleTypes.hh>
#include <G4ParticleTable.hh>
#include <G4UserLimits.hh>
#include <G4EmCalculator.hh>
#include <G4NistManager.hh>
#include <G4HadronicProcessStore.hh>
#include <G4ios.hh>
#include <globals.hh>

#include <iomanip>
#include <G4EmStandardPhysics.hh>
#include <G4EmLivermorePhysics.hh>
#include <G4EmPenelopePhysics.hh>


#include "XAMSPhysicsList.hh"
#include "XAMSPhysicsMessenger.hh"

#include "G4VPhysicsConstructor.hh"
#include "HadronPhysicsQGSP_BERT.hh"
#include "HadronPhysicsQGSP_BERT_HP.hh"
#include "G4EmExtraPhysics.hh"
#include "G4HadronElasticPhysics.hh"
#include "G4HadronElasticPhysicsHP.hh"
#include "G4QStoppingPhysics.hh"
#include "G4IonPhysics.hh"
#include "G4NeutronTrackingCut.hh"

#include "TFile.h"
#include "TH1.h"
#include "TParameter.h"
#include "TNamed.h"

#include <vector>

//___________________________________________________________________________

XAMSPhysicsList::XAMSPhysicsList():G4VUserPhysicsList()
{
  VerboseLevel = 0;
  OpVerbLevel  = 0;
  
  SetVerboseLevel(VerboseLevel);

  
  m_pMessenger = new XAMSPhysicsMessenger(this);
	
  particleList = new G4DecayPhysics("decays");
  
}

//___________________________________________________________________________

XAMSPhysicsList::~XAMSPhysicsList()
{
  MakePhysicsPlots();
  
  delete emPhysicsList;
  delete particleList;
  //	delete opPhysicsList;
  delete m_pMessenger;

  for(size_t i=0; i<hadronPhys.size(); i++) {
    delete hadronPhys[i];
  }
}

//___________________________________________________________________________

void XAMSPhysicsList::ConstructParticle()
{
  //if        (m_hHadronicModel == "custom"){
  ConstructMyBosons();
  ConstructMyLeptons();
  ConstructMyHadrons();
  ConstructMyShortLiveds();
  //} else if (m_hHadronicModel == "QGSP_BERT_HP") {
  particleList->ConstructParticle();
  //}
}

//___________________________________________________________________________

void XAMSPhysicsList::ConstructMyBosons()
{
  G4Geantino::GeantinoDefinition();
  G4ChargedGeantino::ChargedGeantinoDefinition();
  
  G4Gamma::GammaDefinition();
  
  G4OpticalPhoton::OpticalPhotonDefinition();
}

//___________________________________________________________________________

void XAMSPhysicsList::ConstructMyLeptons()
{
  G4Electron::ElectronDefinition();
  G4Positron::PositronDefinition();
  G4MuonPlus::MuonPlusDefinition();
  G4MuonMinus::MuonMinusDefinition();
  
  G4NeutrinoE::NeutrinoEDefinition();
  G4AntiNeutrinoE::AntiNeutrinoEDefinition();
  G4NeutrinoMu::NeutrinoMuDefinition();
  G4AntiNeutrinoMu::AntiNeutrinoMuDefinition();
}

//___________________________________________________________________________

#include "G4MesonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4IonConstructor.hh"

void XAMSPhysicsList::ConstructMyHadrons()
{
  G4MesonConstructor mConstructor;
  mConstructor.ConstructParticle();
  
  G4BaryonConstructor bConstructor;
  bConstructor.ConstructParticle();
  
  G4IonConstructor iConstructor;
  iConstructor.ConstructParticle();
}

//___________________________________________________________________________

void XAMSPhysicsList::ConstructMyShortLiveds()
{

}

//___________________________________________________________________________

#include <G4OpticalPhysics.hh>

void XAMSPhysicsList::ConstructProcess()
{
  G4cout << "XAMSPhysicsList::XAMSPhysicsList() EM physics: "
		<< m_hEMlowEnergyModel << G4endl ;
  G4cout << "XAMSPhysicsList::XAMSPhysicsList() Cerenkov: "
		<< m_bCerenkov << G4endl ;
  
  // First add this one ...
  AddTransportation() ;
  
  // Define EM physics.
  if        (m_hEMlowEnergyModel == "emstandard") {
    emPhysicsList = new G4EmStandardPhysics() ;
  } else if (m_hEMlowEnergyModel == "emlivermore"){
    emPhysicsList = new G4EmLivermorePhysics() ;
  } else if (m_hEMlowEnergyModel == "empenelope"){
    emPhysicsList = new G4EmPenelopePhysics() ;
  } else if (m_hEMlowEnergyModel == "old") {
    G4cout 
			<< "XAMSPhysicsList::ConstructProcess() WARNING: Old version of low energy EM processes ... "
			<< G4endl ;
  } else {
    G4cout 
			<< "XAMSPhysicsList::XAMSPhysicsList() FATAL: Bad EM physics list chosen: "
			<< m_hEMlowEnergyModel << G4endl ;
    G4String msg = " Available choices are: <emstandard> <emlivermore (default)> <empenelope> <old>" ;
    G4Exception("XAMSPhysicsList::ConstructProcess()","PhysicsList",FatalException,msg) ;
  }
  
  // Construct the EM physics processes.
  emPhysicsList->ConstructProcess();

  // Construct optical physics...... is there a G4 standard for this one as well?
  ConstructOp() ;

  //opPhysicsList = new G4OpticalPhysics();
  //opPhysicsList->ConstructProcess();
  
  // Construct the Hadronic physics models.
  //
  hadronPhys.clear();
  if (m_hHadronicModel == "QGSP_BERT") {
    // Implemented QGSP_BERT: is it done in the right way?
    // This follows the recipe from examples/extended/hadronic/Hadr01.
    SetBuilderList1(kFALSE);
    hadronPhys.push_back( new HadronPhysicsQGSP_BERT());
  } else if (m_hHadronicModel == "QGSP_BERT_HP") {
    // Implemented QGSP_BERT_HP: is it done in the right way?
    // This follows the recipe from examples/extended/hadronic/Hadr01.
    SetBuilderList1(kTRUE);
    hadronPhys.push_back( new HadronPhysicsQGSP_BERT_HP());
  } else {
    G4String msg = "XAMSPhysicsList::XAMSPhysicsList() Available choices for Hadronic Physics are: <custom> <QGSP_BERT> <QGSP_BERT_HP>";
    G4Exception("XAMSPhysicsList::ConstructProcess()","PhysicsList",FatalException,msg);
  }

  // Construct processes.
  for(size_t i=0; i<hadronPhys.size(); i++) {
    hadronPhys[i]->ConstructProcess();
  }
  
  // Construct other processes.
  particleList->ConstructProcess();
}

//___________________________________________________________________________

void XAMSPhysicsList::SetBuilderList1(G4bool flagHP)
{
  hadronPhys.push_back( new G4EmExtraPhysics());
  if(flagHP) {
    hadronPhys.push_back( new G4HadronElasticPhysicsHP() );
  } else {
    hadronPhys.push_back( new G4HadronElasticPhysics() );
  }
  hadronPhys.push_back( new G4QStoppingPhysics());
  hadronPhys.push_back( new G4IonPhysics());
  hadronPhys.push_back( new G4NeutronTrackingCut());
}

//___________________________________________________________________________

void XAMSPhysicsList::AddTransportation()
{
  G4VUserPhysicsList::AddTransportation();
}

//___________________________________________________________________________

// Optical Processes ////////////////////////////////////////////////////////
#include "G4Scintillation.hh"
#include "G4OpAbsorption.hh"
#include "G4OpRayleigh.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4Cerenkov.hh"

void XAMSPhysicsList::ConstructOp()
{
	// Scintillation yields according to XENON standards.
  // Default scintillation process.
  G4Scintillation *theScintProcessDef = new G4Scintillation("Scintillation") ;
  //theScintProcessDef->DumpPhysicsTable();
  theScintProcessDef->SetTrackSecondariesFirst(true) ;
  theScintProcessDef->SetScintillationYieldFactor(1.0) ;
  theScintProcessDef->SetScintillationExcitationRatio(0.0) ;
  theScintProcessDef->SetVerboseLevel(OpVerbLevel) ;
  
  // Scintillation process for alpha.
  G4Scintillation *theScintProcessAlpha = new G4Scintillation("Scintillation") ;
  //theScintProcessNuc->DumpPhysicsTable();
  theScintProcessAlpha->SetTrackSecondariesFirst(true) ;
  theScintProcessAlpha->SetScintillationYieldFactor(1.1) ;
  theScintProcessAlpha->SetScintillationExcitationRatio(1.0) ;
  theScintProcessAlpha->SetVerboseLevel(OpVerbLevel) ;
  
  // Scintillation process for heavy nuclei.
  G4Scintillation *theScintProcessNuc = new G4Scintillation("Scintillation") ;
  //theScintProcessNuc->DumpPhysicsTable();
  theScintProcessNuc->SetTrackSecondariesFirst(true) ;
  theScintProcessNuc->SetScintillationYieldFactor(0.2) ;
  theScintProcessNuc->SetScintillationExcitationRatio(1.0) ;
  theScintProcessNuc->SetVerboseLevel(OpVerbLevel) ;
  
  // Add Cerenkov.
  G4Cerenkov *fCerenkovProcess = new G4Cerenkov("Cerenkov") ;
  
  if (m_bCerenkov) {
    G4cout << "XAMSPhysicsList::ConstructOp() Define Cerenkov .... " << G4endl ;
    // The maximum number of photons per GEANT4 step.
    G4double fMaxNumPhotons = 100; // same as in G4OpticalPhysics.cc I think, but no guarantees
    // The maximum change in beta=v/c in percent.
    G4double fMaxBetaChange = 10;  // Same as in G4OpticalPhysics.cc.
    // Tracks secondaries before continuing with the original particle.
    G4bool fTrackSecondariesFirst = true; // Same as in G4OpticalPhysics.cc.
    
    fCerenkovProcess->SetMaxNumPhotonsPerStep(fMaxNumPhotons);
    fCerenkovProcess->SetMaxBetaChangePerStep(fMaxBetaChange);
    fCerenkovProcess->SetTrackSecondariesFirst(fTrackSecondariesFirst);
  } else {
    G4cout << "XAMSPhysicsList::ConstructOp() Disable Cerenkov .... " << G4endl ;
  }
  
	// Optical processes.
  G4OpAbsorption *theAbsorptionProcess       = new G4OpAbsorption();
  G4OpRayleigh* theRayleighScatteringProcess = new G4OpRayleigh();
  G4OpBoundaryProcess *theBoundaryProcess    = new G4OpBoundaryProcess();
	
  //theAbsorptionProcess->DumpPhysicsTable();
  //theRayleighScatteringProcess->DumpPhysicsTable();
  theAbsorptionProcess->SetVerboseLevel(OpVerbLevel);
  theRayleighScatteringProcess->SetVerboseLevel(OpVerbLevel);
  theBoundaryProcess->SetVerboseLevel(OpVerbLevel);
  G4OpticalSurfaceModel themodel = unified;
  
  theBoundaryProcess->SetModel(themodel);
  
  theParticleIterator->reset();
  while((*theParticleIterator) ())
  {
    G4ParticleDefinition *particle = theParticleIterator->value();
    G4ProcessManager *pmanager = particle->GetProcessManager();
    G4String particleName = particle->GetParticleName();
    
    if(theScintProcessDef->IsApplicable(*particle))
    {
      //      if(particle->GetPDGMass() > 5.0*GeV) 
      if(particle->GetParticleName() == "GenericIon")
	    {
	      pmanager->AddProcess(theScintProcessNuc);	// AtRestDiscrete
	      pmanager->SetProcessOrderingToLast(theScintProcessNuc,
                                           idxAtRest);
	      pmanager->SetProcessOrderingToLast(theScintProcessNuc,
                                           idxPostStep);
	    }
      else if(particle->GetParticleName() == "alpha")
	    {
	      pmanager->AddProcess(theScintProcessAlpha);
	      pmanager->SetProcessOrderingToLast(theScintProcessAlpha,
                                           idxAtRest);
	      pmanager->SetProcessOrderingToLast(theScintProcessAlpha,
                                           idxPostStep);
	    }
      else
	    {
	      pmanager->AddProcess(theScintProcessDef);
	      pmanager->SetProcessOrderingToLast(theScintProcessDef,
                                           idxAtRest);
	      pmanager->SetProcessOrderingToLast(theScintProcessDef,
                                           idxPostStep);
	    }
    }
    // Give the optical photons some TLC.
    if(particleName == "opticalphoton")
    {
      pmanager->AddDiscreteProcess(theAbsorptionProcess);
      pmanager->AddDiscreteProcess(theRayleighScatteringProcess);
      pmanager->AddDiscreteProcess(theBoundaryProcess);
    }
    // ... and give those particles that need it a bit of Cerenkov... and only if you want to
    if(fCerenkovProcess->IsApplicable(*particle) && m_bCerenkov){
      pmanager->AddProcess(fCerenkovProcess);
      pmanager->SetProcessOrdering(fCerenkovProcess,idxPostStep);
    }
    //
  }
}

// Cuts /////////////////////////////////////////////////////////////////////
void XAMSPhysicsList::SetCuts()
{
	// Range cut for tracking particles.
  defaultCutValue = m_pTrackingCut ;	// From messenger. 
  cutForGamma     = defaultCutValue;
  cutForElectron  = defaultCutValue;
  cutForPositron  = defaultCutValue;
  
  if(verboseLevel > 1)
    G4cout << "XAMSPhysicsList::SetCuts:" ;
  
  // Cut for production of secondaries, special for low energy physics.
  G4double lowlimit = m_pProductionCut ;
  
  G4ProductionCutsTable::GetProductionCutsTable()
		->SetEnergyRange(lowlimit,100. * GeV) ;
  
  // Set cut values for gamma first, for e- second and next for e+,
  // because some processes for e+/e- need cut values for gamma.
  SetCutValue(cutForGamma, "gamma") ;
  SetCutValue(cutForElectron, "e-") ;
  SetCutValue(cutForPositron, "e+") ;
  
  if(verboseLevel > 0)
  {
    G4cout << "XAMSPhysicsList::SetCuts:" ;
    G4cout << "RangeCut : "
			<< defaultCutValue / mm << " mm" << G4endl ;
    G4cout << "PoductionCut : "
			<< lowlimit / eV << " eV" << G4endl ;
    DumpCutValuesTable() ;
	}
}

//___________________________________________________________________________

void XAMSPhysicsList::MakePhysicsPlots() {
  G4cout << "XAMSPhysicsList:: Init directory structure for histogramming..." << G4endl ;
  TFile *_f_physics = new TFile("physics.root","RECREATE");
  
  TDirectory *_physics = _f_physics->mkdir("physics");
  TDirectory *_em_physics=_physics->mkdir("electromagnetic");
  TDirectory *_had_physics=_physics->mkdir("hadronic");
  
  
  // make a list of materials for graphs
  G4int nmaterial = G4Material::GetNumberOfMaterials(); 
  G4cout <<"MakePhysicsPlots:: Number of materials = "<<nmaterial<<G4endl;
  
  //
  // EM physics plots
  //
  
  //
  // plotting range on a xlog-scale
  //
  G4double emin=0.001;  // 
  G4double emax=20.00; // 20 MeV
  emin = std::log10(emin);
  emax = std::log10(emax);
  
  G4int    nstep = 1000;
  G4double de    = (emax - emin)/nstep; // 
  
  _em_physics->cd();
  //TParameter<double> *_EMmodelPar = new TParameter<double>("EMmodel",1.0);
  TNamed *_EMmodelPar = new TNamed("EMmodel",m_hEMlowEnergyModel);
  _EMmodelPar->Write();
  
  G4EmCalculator emCalc;
  
  std::vector<G4String> matNames,procNames;
  
  std::vector<TH1F*> _compt;
  std::vector<TH1F*> _phot;
  std::vector<TH1F*> _conv;
  
  //  for(G4int imat=0; imat<(G4int)matNames.size(); imat++){
  for(G4int imat=0; imat<nmaterial; imat++){
    // get material
    G4Material *mat = G4NistManager::Instance()->GetMaterial(imat);
    G4double    rho = mat->GetDensity()/(g/cm3);
    G4cout << "MakePhysicsPlots:: Generate EM plots for:"<< mat->GetName()<<G4endl;
    
    // book histograms
    char  hName[100];
    sprintf(hName,"%s_compt",mat->GetName().c_str());
    _compt.push_back(new TH1F(hName,hName,nstep,emin-de/2,emax-de/2));
    sprintf(hName,"%s_phot",mat->GetName().c_str());
    _phot.push_back(new TH1F(hName,hName,nstep,emin-de/2,emax-de/2));
    sprintf(hName,"%s_conv",mat->GetName().c_str());
    _conv.push_back(new TH1F(hName,hName,nstep,emin-de/2,emax-de/2));
    
    // fill histograms - skip the 0 bin
    
    for(G4int i=0; i<nstep; i++){
      G4double x=emin+i*de;
      G4double e=std::pow(10,x);
      // cross section in cm2/g!
      G4double sig_compt = emCalc.ComputeCrossSectionPerVolume(e,"gamma","compt",mat->GetName(),0)*cm / rho;
      G4double sig_phot  = emCalc.ComputeCrossSectionPerVolume(e,"gamma","phot",mat->GetName(),0)*cm / rho;
      G4double sig_conv  = emCalc.ComputeCrossSectionPerVolume(e,"gamma","conv",mat->GetName(),0)*cm / rho;
      _compt[imat]->Fill(x,sig_compt);
      _phot[imat]->Fill(x,sig_phot);
      _conv[imat]->Fill(x,sig_conv);
    }// fill histograms
  }// loop over materials
  
  // Hadronic physics plots:: only neutrons.....
  //
  // plotting range on a xlog-scale
  //
  emin=0.00000001;  // 
  emax=20.00; // 20 MeV
  emin = std::log10(emin);
  emax = std::log10(emax);
  
  nstep = 1000;
  de    = (emax - emin)/nstep; // 
  
  _had_physics->cd();
  TNamed *_HADmodelPar = new TNamed("HADmodel",m_hHadronicModel);
  _HADmodelPar->Write();
  // histograms
  std::vector<TH1F*> _neutron_elastic;
  std::vector<TH1F*> _neutron_inelastic;
  std::vector<TH1F*> _neutron_capture;
  std::vector<TH1F*> _neutron_fission;
  
  G4HadronicProcessStore* hadstore     = G4HadronicProcessStore::Instance(); 
  const G4ParticleDefinition* particle = G4ParticleTable::GetParticleTable()->FindParticle("neutron");
  
  for(G4int imat=0; imat<nmaterial; imat++){
    // get material
    G4Material *mat = G4NistManager::Instance()->GetMaterial(imat);
    G4cout << "MakePhysicsPlots:: Generate neutron cross section plots for:"<< mat->GetName()<<G4endl;
    
    // book histograms
    char  hName[100];
    sprintf(hName,"%s_neutron_elastic",mat->GetName().c_str());
    _neutron_elastic.push_back(new TH1F(hName,hName,nstep,emin-de/2,emax-de/2));
    sprintf(hName,"%s_neutron_inelastic",mat->GetName().c_str());
    _neutron_inelastic.push_back(new TH1F(hName,hName,nstep,emin-de/2,emax-de/2));
    sprintf(hName,"%s_neutron_capture",mat->GetName().c_str());
    _neutron_capture.push_back(new TH1F(hName,hName,nstep,emin-de/2,emax-de/2));
    sprintf(hName,"%s_neutron_fission",mat->GetName().c_str());
    _neutron_fission.push_back(new TH1F(hName,hName,nstep,emin-de/2,emax-de/2));
    
    for(G4int i=0; i<nstep; i++){
      G4double x=emin+i*de;
      G4double e=std::pow(10.,x);
      G4double xs_elastic   = hadstore->GetElasticCrossSectionPerVolume(particle,e,mat)*cm;
      G4double xs_inelastic = hadstore->GetInelasticCrossSectionPerVolume(particle,e,mat)*cm;
      G4double xs_capture   = hadstore->GetCaptureCrossSectionPerVolume(particle,e,mat)*cm;
      G4double xs_fission   = hadstore->GetFissionCrossSectionPerVolume(particle,e,mat)*cm;
      _neutron_elastic[imat]->Fill(x,xs_elastic);
      _neutron_inelastic[imat]->Fill(x,xs_inelastic);
      _neutron_capture[imat]->Fill(x,xs_capture);
      _neutron_fission[imat]->Fill(x,xs_fission);
    }
  }
  
  //
  // Neutron cross sections for elements
  //
  // histograms
  std::vector<TH1F*> _el_neutron_elastic;
  std::vector<TH1F*> _el_neutron_inelastic;
  std::vector<TH1F*> _el_neutron_capture;
  std::vector<TH1F*> _el_neutron_fission;
  
  
  G4int nelement = G4Element::GetNumberOfElements();
  G4cout <<" number of elements "<< nelement<<G4endl;
  
  for(G4int iel = 0; iel<nelement; iel++){
    G4Element *ele = G4NistManager::Instance()->GetElement(iel);
    G4cout <<"MakePhysicsPlots:: element = "<<ele->GetName()<<G4endl;
    
    // book histograms
    char  hName[100];
    sprintf(hName,"_el_%s_neutron_elastic",ele->GetName().c_str());
    _el_neutron_elastic.push_back(new TH1F(hName,hName,nstep,emin-de/2,emax-de/2));
    sprintf(hName,"_el_%s_neutron_inelastic",ele->GetName().c_str());
    _el_neutron_inelastic.push_back(new TH1F(hName,hName,nstep,emin-de/2,emax-de/2));
    sprintf(hName,"_el_%s_neutron_capture",ele->GetName().c_str());
    _el_neutron_capture.push_back(new TH1F(hName,hName,nstep,emin-de/2,emax-de/2));
    sprintf(hName,"_el_%s_neutron_fission",ele->GetName().c_str());
    _el_neutron_fission.push_back(new TH1F(hName,hName,nstep,emin-de/2,emax-de/2));
    
    G4Material *mdummy = G4NistManager::Instance()->GetMaterial(1);
    for(G4int i=0; i<nstep; i++){
      G4double x=emin+i*de;
      G4double e=std::pow(10.,x);
      G4double xs_elastic   = hadstore->GetElasticCrossSectionPerAtom(particle,e,ele,mdummy)/barn;
      G4double xs_inelastic = hadstore->GetInelasticCrossSectionPerAtom(particle,e,ele,mdummy)/barn;
      G4double xs_capture   = hadstore->GetCaptureCrossSectionPerAtom(particle,e,ele,mdummy)/barn;
      G4double xs_fission   = hadstore->GetFissionCrossSectionPerAtom(particle,e,ele,mdummy)/barn;
      _el_neutron_elastic[iel]->Fill(x,xs_elastic);
      _el_neutron_inelastic[iel]->Fill(x,xs_inelastic);
      _el_neutron_capture[iel]->Fill(x,xs_capture);
      _el_neutron_fission[iel]->Fill(x,xs_fission);
    }
  }
  
  _physics->cd();
  
  // write the names of the Geant4 external data sets to the root output
  WriteParameter("G4NEUTRONHPDATA");
  WriteParameter("G4LEDATA");
  WriteParameter("G4LEVELGAMMADATA");
  WriteParameter("G4NEUTRONXSDATA");
  WriteParameter("G4RADIOACTIVEDATA");
  WriteParameter("G4ABLADATA");
  WriteParameter("G4PIIDATA");
  WriteParameter("G4REALSURFACEDATA");
  
  //
  // return to root directory
  //
  _f_physics->cd();
  _f_physics->Write();
  _f_physics->Close();
}

//__________________________________________________________________________

void XAMSPhysicsList::WriteParameter(G4String parName)
{
  char * pch     = strtok ( getenv(parName.c_str()), "/" );
  char * parVal  = pch;
  // split string to keep only last part of path.... (handling strings in c++ is not fun)
  while (pch != NULL) {
    parVal = pch;
    pch = strtok (NULL, "/");
  }
  TNamed *Parameter = new TNamed(parName.c_str(),parVal);
  Parameter->Write();
}

//__________________________________________________________________________
