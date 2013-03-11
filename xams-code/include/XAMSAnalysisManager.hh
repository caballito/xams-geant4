#ifndef __ANALYSISMANAGER_H__
#define __ANALYSISMANAGER_H__

#include <globals.hh>

#include <TParameter.h>
#include <TDirectory.h>
#include <G4Timer.hh>
 
class G4Run;
class G4Event;
class G4Step;

class TFile;
class TTree;

class XAMSEventData;
class XAMSPrimaryGeneratorAction;

class XAMSAnalysisManager
{
public:
  XAMSAnalysisManager(XAMSPrimaryGeneratorAction *pPrimaryGeneratorAction);
  virtual ~XAMSAnalysisManager();

public:
  virtual void BeginOfRun(const G4Run *pRun); 
  virtual void EndOfRun(const G4Run *pRun); 
  virtual void BeginOfEvent(const G4Event *pEvent); 
  virtual void EndOfEvent(const G4Event *pEvent); 
  virtual void Step(const G4Step *pStep);	
  
  void SetDataFilename(const G4String &hFilename) { m_hDataFilename = hFilename; }
  void SetNbEventsToSimulate(G4int iNbEventsToSimulate) { m_iNbEventsToSimulate = iNbEventsToSimulate;}

private:
  G4bool FilterEvent(XAMSEventData *pEventData);

private:
  G4int m_iHitsCollectionID;

  G4String m_hDataFilename;
  G4int m_iNbEventsToSimulate;

  TFile      *m_pTreeFile;
  TTree      *m_pTree;
  TDirectory *_events;

  TParameter<int> *m_pNbEventsToSimulateParameter;
  //  TParameter<double> *nRejectParameter;
  // TParameter<double> *nAcceptParameter;

  XAMSPrimaryGeneratorAction *m_pPrimaryGeneratorAction;

  XAMSEventData *m_pEventData;
  G4bool              plotPhysics;

  G4Timer *runTime;
//  G4bool            ForcedTransport;
};

#endif // __XENON10PANALYSISMANAGER_H__

