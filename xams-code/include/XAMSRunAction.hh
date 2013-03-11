#ifndef __XENON10PRUNACTION_H__
#define __XENON10PRUNACTION_H__

#include <G4UserRunAction.hh>
#include "XAMSRunActionMessenger.hh"

class G4Run;

class XAMSAnalysisManager;


class XAMSRunAction: public G4UserRunAction
{
public:
	XAMSRunAction(XAMSAnalysisManager *pAnalysisManager=0);
	~XAMSRunAction();

public:
	void BeginOfRunAction(const G4Run *pRun);
	void EndOfRunAction(const G4Run *pRun);
        
	void SetRanSeed(G4int hRanSeed) { m_hRanSeed = hRanSeed; }
//	void SetForcedTransport(G4bool doit) { m_hForcedTransport = doit; }

private:
	G4int m_hRanSeed;
//        G4bool m_hForcedTransport;
	XAMSAnalysisManager *m_pAnalysisManager;
	XAMSRunActionMessenger *m_pMessenger;
};

#endif // __XENON10PRUNACTION_H__

