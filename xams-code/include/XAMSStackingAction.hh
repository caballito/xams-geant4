#ifndef __STACKINGACTION_H__
#define __STACKINGACTION_H__

#include <globals.hh>
#include <G4UserStackingAction.hh>

class XAMSAnalysisManager;

class XAMSStackingAction: public G4UserStackingAction
{
public:
	XAMSStackingAction(XAMSAnalysisManager *pAnalysisManager=0);
	~XAMSStackingAction();
  
	virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* aTrack);
	virtual void NewStage();
	virtual void PrepareNewEvent();

private:
	XAMSAnalysisManager *m_pAnalysisManager;
};

#endif // 

