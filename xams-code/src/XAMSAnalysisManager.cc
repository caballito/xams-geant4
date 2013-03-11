#include <G4SDManager.hh>
#include <G4Run.hh>
#include <G4Event.hh>
#include <G4HCofThisEvent.hh>
#include <G4EmCalculator.hh>
#include <G4Material.hh>
#include <G4HadronicProcessStore.hh>
#include <G4ParticleTable.hh>
#include <G4NistManager.hh>
#include <G4ElementTable.hh>
#include <G4Version.hh>
//#include <G4Element.hh> 
#include <numeric>

#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TParameter.h>
#include <TDirectory.h>
#include <TH1.h>

#include "XAMSDetectorConstruction.hh"
#include "XAMSHit.hh"
#include "XAMSPrimaryGeneratorAction.hh"
#include "XAMSEventData.hh"

#include "XAMSAnalysisManager.hh"

XAMSAnalysisManager::XAMSAnalysisManager(XAMSPrimaryGeneratorAction *pPrimaryGeneratorAction)
{
  runTime = new G4Timer();
  
  m_iHitsCollectionID = -1;
  m_hDataFilename = "events.root";
  
  m_pPrimaryGeneratorAction = pPrimaryGeneratorAction;
  
  m_pEventData = new XAMSEventData();
  plotPhysics = kTRUE;
}

XAMSAnalysisManager::~XAMSAnalysisManager()
{
}

void
XAMSAnalysisManager::BeginOfRun(const G4Run *)
{
  // start a timer for this run....
  runTime->Start();
  
  m_pTreeFile = new TFile(m_hDataFilename.c_str(), "RECREATE");//, "File containing event data for Xenon1T");
  // make tree structure
  TNamed *G4version = new TNamed("G4VERSION_TAG",G4VERSION_TAG);
  G4version->Write();
  
  _events = m_pTreeFile->mkdir("events");
  _events->cd();
  
  G4cout <<"XAMSAnalysisManager:: Init data tree ..."<<G4endl;
  m_pTree = new TTree("events", "Tree containing event data for XAMS experiment");
  
  gROOT->ProcessLine("#include <vector>");
  
  m_pTree->Branch("eventid", &m_pEventData->m_iEventId, "eventid/I");
  m_pTree->Branch("etot",    &m_pEventData->m_fTotalEnergyDeposited, "etot/F");
  m_pTree->Branch("nsteps",  &m_pEventData->m_iNbSteps, "nsteps/I");
	
  m_pTree->Branch("trackid",   "vector<int>",   &m_pEventData->m_pTrackId);
  m_pTree->Branch("type",      "vector<string>",&m_pEventData->m_pParticleType);
  m_pTree->Branch("parentid",  "vector<int>",   &m_pEventData->m_pParentId);
  m_pTree->Branch("parenttype","vector<string>",&m_pEventData->m_pParentType);
  m_pTree->Branch("creaproc",  "vector<string>",&m_pEventData->m_pCreatorProcess);
  m_pTree->Branch("edproc",    "vector<string>",&m_pEventData->m_pDepositingProcess);
  m_pTree->Branch("xp",        "vector<float>", &m_pEventData->m_pX);
  m_pTree->Branch("yp",        "vector<float>", &m_pEventData->m_pY);
  m_pTree->Branch("zp",        "vector<float>", &m_pEventData->m_pZ);
  m_pTree->Branch("ed",        "vector<float>", &m_pEventData->m_pEnergyDeposited);
  m_pTree->Branch("ekin",      "vector<float>", &m_pEventData->m_pKineticEnergy);
  m_pTree->Branch("time",      "vector<float>", &m_pEventData->m_pTime);
  m_pTree->Branch("steplen",   "vector<float>", &m_pEventData->m_pStepLength);
  
  m_pTree->Branch("type_pri",  "vector<string>",&m_pEventData->m_pPrimaryParticleType);
  m_pTree->Branch("xp_pri", &m_pEventData->m_fPrimaryX, "xp_pri/F");
  m_pTree->Branch("yp_pri", &m_pEventData->m_fPrimaryY, "yp_pri/F");
  m_pTree->Branch("zp_pri", &m_pEventData->m_fPrimaryZ, "zp_pri/F");
  m_pTree->Branch("e_pri",  &m_pEventData->m_fPrimaryE, "e_pri/F");
  m_pTree->Branch("w_pri",  &m_pEventData->m_fPrimaryW, "w_pri/F");
  
  
  //  m_pTree->SetMaxTreeSize(G4int(10e9));
  //  m_pTree->AutoSave();
  
  m_pNbEventsToSimulateParameter = new TParameter<int>("nbevents", m_iNbEventsToSimulate);
  m_pNbEventsToSimulateParameter->Write();
  
  m_pTreeFile->cd();
  
}

void
XAMSAnalysisManager::EndOfRun(const G4Run *)
{
  runTime->Stop();
  G4double dt = runTime->GetRealElapsed();
  // make tree structure
  TParameter<G4double> *dtPar = new TParameter<G4double>("G4RUNTIME",dt);
  dtPar->Write();
  
  m_pTreeFile->cd();
  
  m_pTreeFile->Write();
  m_pTreeFile->Close();
}

void
XAMSAnalysisManager::BeginOfEvent(const G4Event *)
{
  if(m_iHitsCollectionID == -1)
  {
    G4SDManager *pSDManager = G4SDManager::GetSDMpointer();
    m_iHitsCollectionID = pSDManager->GetCollectionID("HitsCollection");
  } 
  
  //  if(m_iPmtHitsCollectionID == -1)
  //  {
  //  G4SDManager *pSDManager = G4SDManager::GetSDMpointer();
  //  m_iPmtHitsCollectionID = pSDManager->GetCollectionID("PmtHitsCollection");
  // }
}

void
XAMSAnalysisManager::EndOfEvent(const G4Event *pEvent)
{
  _events->cd();
  
  G4HCofThisEvent* pHCofThisEvent = pEvent->GetHCofThisEvent();
  XAMSHitsCollection* pHitsCollection = 0;
  // Xenon1tPmtHitsCollection* pPmtHitsCollection = 0;
  
  G4int iNbHits = 0;
	
  if(pHCofThisEvent)
  {
    if(m_iHitsCollectionID != -1)
    {
      pHitsCollection = (XAMSHitsCollection *)(pHCofThisEvent->GetHC(m_iHitsCollectionID));
      iNbHits = (pHitsCollection)?(pHitsCollection->entries()):(0);
    }
  }
  
  // get the event ID and primary particle information
  m_pEventData->m_iEventId = pEvent->GetEventID();
  m_pEventData->m_pPrimaryParticleType->push_back(m_pPrimaryGeneratorAction->GetParticleTypeOfPrimary());
  
  m_pEventData->m_fPrimaryX = m_pPrimaryGeneratorAction->GetPositionOfPrimary().x();
  m_pEventData->m_fPrimaryY = m_pPrimaryGeneratorAction->GetPositionOfPrimary().y();
  m_pEventData->m_fPrimaryZ = m_pPrimaryGeneratorAction->GetPositionOfPrimary().z();
  
  m_pEventData->m_fPrimaryE = m_pPrimaryGeneratorAction->GetEnergyOfPrimary() / keV;
  m_pEventData->m_fPrimaryW = pEvent->GetPrimaryVertex()->GetWeight();
  
  
  G4int iNbSteps = 0;
  G4float fTotalEnergyDeposited = 0.;
  
  if(iNbHits)
  {
    // LXe hits
    for(G4int i=0; i<iNbHits; i++)
    {
      XAMSHit *pHit = (*pHitsCollection)[i];
      
      if(pHit->GetParticleType() != "opticalphoton")
	    {
	      m_pEventData->m_pTrackId->push_back(pHit->GetTrackId());
	      m_pEventData->m_pParentId->push_back(pHit->GetParentId());
        
	      m_pEventData->m_pParticleType->push_back(pHit->GetParticleType());
	      m_pEventData->m_pParentType->push_back(pHit->GetParentType());
	      m_pEventData->m_pCreatorProcess->push_back(pHit->GetCreatorProcess());
	      m_pEventData->m_pDepositingProcess->push_back(pHit->GetDepositingProcess());
        
	      m_pEventData->m_pX->push_back(pHit->GetPosition().x()/mm);
	      m_pEventData->m_pY->push_back(pHit->GetPosition().y()/mm);
	      m_pEventData->m_pZ->push_back(pHit->GetPosition().z()/mm);
        
	      fTotalEnergyDeposited += pHit->GetEnergyDeposited()/keV;
	      m_pEventData->m_pEnergyDeposited->push_back(pHit->GetEnergyDeposited()/keV);
        
	      m_pEventData->m_pKineticEnergy->push_back(pHit->GetKineticEnergy()/keV);
	      m_pEventData->m_pTime->push_back(pHit->GetTime()/second);
	      m_pEventData->m_pStepLength->push_back(pHit->GetStepLength()/mm);
        
	      iNbSteps++;
	    }
    }
  }
  //APC		if(fTotalEnergyDeposited > 0. || iNbPmtHits > 0)
  
  // also write the header information + primary vertex of the empty events....
  m_pEventData->m_iNbSteps = iNbSteps;
  m_pEventData->m_fTotalEnergyDeposited = fTotalEnergyDeposited;
  
  m_pTree->Fill();
  m_pEventData->Clear(); 
  
  m_pTreeFile->cd();
}

void
XAMSAnalysisManager::Step(const G4Step *)
{
  
}
