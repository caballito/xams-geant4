#ifndef __EVENTACTION_H__
#define __EVENTACTION_H__

#include <G4UserEventAction.hh>

#include "XAMSAnalysisManager.hh"

class G4Event;

class XAMSEventAction : public G4UserEventAction
{
public:
	XAMSEventAction(XAMSAnalysisManager *pAnalysisManager = 0);
	~XAMSEventAction();

public:
	void BeginOfEventAction(const G4Event *pEvent);
	void EndOfEventAction(const G4Event *pEvent);

private:
	XAMSAnalysisManager *m_pAnalysisManager;
};

#endif // __EVENTACTION_H__

