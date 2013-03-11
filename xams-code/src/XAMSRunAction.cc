#include <Randomize.hh>

#include <sys/time.h>

#include "XAMSAnalysisManager.hh"

#include "XAMSRunAction.hh"

XAMSRunAction::XAMSRunAction(XAMSAnalysisManager *pAnalysisManager)
{
  m_hRanSeed         = 12345; // default value
  m_pMessenger       = new XAMSRunActionMessenger(this);
  
  m_pAnalysisManager = pAnalysisManager;
  
}

XAMSRunAction::~XAMSRunAction()
{
  delete m_pMessenger;
}

void
XAMSRunAction::BeginOfRunAction(const G4Run *pRun)
{
  if(m_pAnalysisManager) {
    //    m_pAnalysisManager->SetForcedTransport(m_hForcedTransport);
    m_pAnalysisManager->BeginOfRun(pRun);
  }
  
  //	struct timeval hTimeValue;
  //	gettimeofday(&hTimeValue, NULL);
  
  //  CLHEP::HepRandom::setTheEngine(new CLHEP::DRand48Engine);
  //  CLHEP::HepRandom::setTheEngine(new CLHEP::HepRandomEnginee);
  CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);
  //  	CLHEP::HepRandom::setTheSeed(hTimeValue.tv_usec);
  G4cout << "XAMSRunAction::BeginOfRunAction Initialize random numbers with seed = "<<m_hRanSeed<<G4endl;
  CLHEP::HepRandom::setTheSeed(m_hRanSeed);
}

void
XAMSRunAction::EndOfRunAction(const G4Run *pRun)
{
  if(m_pAnalysisManager)
    m_pAnalysisManager->EndOfRun(pRun);
}

