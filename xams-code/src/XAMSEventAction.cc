#include <G4Event.hh>

#include "XAMSEventAction.hh"

XAMSEventAction::XAMSEventAction(XAMSAnalysisManager *pAnalysisManager)
{
  m_pAnalysisManager = pAnalysisManager;
}

XAMSEventAction::~XAMSEventAction()
{
}

void
XAMSEventAction::BeginOfEventAction(const G4Event *pEvent)
{
  if(pEvent->GetEventID() % 1000 == 0)
    {
      G4cout << "------ Begin event " << pEvent->GetEventID()<< "------" << G4endl;
    }
  
  if(m_pAnalysisManager)
    m_pAnalysisManager->BeginOfEvent(pEvent);
}

void XAMSEventAction::EndOfEventAction(const G4Event *pEvent)
{
  if(m_pAnalysisManager)
    m_pAnalysisManager->EndOfEvent(pEvent);
}


