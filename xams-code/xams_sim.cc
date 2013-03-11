#include <string>
#include <sstream>
#include <unistd.h>

#include <G4RunManager.hh>
#include <G4UImanager.hh>
#include <G4UIterminal.hh>
#include <G4UItcsh.hh>
#include <G4VisExecutive.hh>
#include <G4GeneralParticleSource.hh>

#include "XAMSDetectorConstruction.hh"
#include "XAMSPhysicsList.hh"
#include "XAMSPrimaryGeneratorAction.hh"

#include "XAMSAnalysisManager.hh"
#include "XAMSStackingAction.hh"
#include "XAMSRunAction.hh"
#include "XAMSEventAction.hh"

#include "fileMerger.hh"

void usage();

int
main(int argc, char **argv)
{
  // switches
  int c = 0;
  
  std::stringstream hStream;
	
  bool bInteractive = false;
  bool bVisualize = false;
  bool bVrmlVisualize = false;
  bool bOpenGlVisualize = false;
  bool bPreInitFromFile = false;
	
  bool bMacroFile = false;
  std::string hMacroFilename, hDataFilename, hPreInitFilename;
  std::string hCommand;
  int iNbEventsToSimulate = 0;
  
  // parse switches
  while((c = getopt(argc,argv,"v:f:o:p:n:i")) != -1)
  {
    switch(c)	{
        
      case 'v': 
        bVisualize = true;
        hStream.str(optarg);
        if(hStream.str() == "vrml")
          bVrmlVisualize = true;
        else if(hStream.str() == "opengl")
          bOpenGlVisualize = true;
        hStream.clear();
        break;
        
      case 'f':
        bMacroFile = true;
        hMacroFilename = optarg;
        break;
        
      case 'o':
        hDataFilename = optarg;
        break;
				
      case 'p':
        bPreInitFromFile = true;
        hPreInitFilename = optarg;
        break;
        
      case 'n':
        hStream.str(optarg);
        hStream.clear();
        hStream >> iNbEventsToSimulate;
        break;
        
      case 'i':
        bInteractive = true;
        break;
        
      default:
        usage();
    }
  }
  
  // create the run manager
  G4RunManager *pRunManager = new G4RunManager;
	
  // set user-defined initialization classes
  pRunManager->SetUserInitialization(new XAMSDetectorConstruction);
  pRunManager->SetUserInitialization(new XAMSPhysicsList);
	
  G4VisManager* pVisManager = new G4VisExecutive;
  pVisManager->Initialize();
  
  // create the primary generator action
  XAMSPrimaryGeneratorAction *pPrimaryGeneratorAction = new XAMSPrimaryGeneratorAction();
  // create an analysis manager object
  XAMSAnalysisManager *pAnalysisManager = new XAMSAnalysisManager(pPrimaryGeneratorAction);
  if(!hDataFilename.empty())
    //pAnalysisManager->SetDataFilename(Form("/tmp/%s",hDataFilename));
    pAnalysisManager->SetDataFilename(hDataFilename);
  if(iNbEventsToSimulate) pAnalysisManager->SetNbEventsToSimulate(iNbEventsToSimulate);
	
  // set user-defined action classes
  pRunManager->SetUserAction(pPrimaryGeneratorAction);
  pRunManager->SetUserAction(new XAMSStackingAction(pAnalysisManager));
  pRunManager->SetUserAction(new XAMSRunAction(pAnalysisManager));
  pRunManager->SetUserAction(new XAMSEventAction(pAnalysisManager));
  
  // geometry IO
  G4UImanager* pUImanager = G4UImanager::GetUIpointer();
  
  if(bPreInitFromFile)
  {
    hCommand = "/control/execute " + hPreInitFilename;
    pUImanager->ApplyCommand(hCommand);
  }
	
  // initialize it all....
  pRunManager->Initialize();
  
  G4UIsession * pUIsession = 0;
  if(bInteractive) pUIsession = new G4UIterminal(new G4UItcsh);
  
  if(bVisualize)
  {
    pUImanager->ApplyCommand("/vis/scene/create");
    if(bVrmlVisualize)
      pUImanager->ApplyCommand("/vis/open VRML2FILE");
    if(bOpenGlVisualize)
      pUImanager->ApplyCommand("/vis/open OGLIX");
		
    pUImanager->ApplyCommand("/vis/viewer/set/viewpointThetaPhi 90 0 deg");
    pUImanager->ApplyCommand("/vis/viewer/set/upVector 0 0 1");
    pUImanager->ApplyCommand("/vis/viewer/zoom 1.0");
    pUImanager->ApplyCommand("/tracking/storeTrajectory 1");
    pUImanager->ApplyCommand("/vis/scene/add/trajectories");
  }
  
  // run time parameter settings
  if(bMacroFile)
  {
    hCommand = "/control/execute " + hMacroFilename;
    pUImanager->ApplyCommand(hCommand);
  }
	
  
  if(iNbEventsToSimulate)
  {
    hStream.str("");
    hStream.clear();
    hStream << "/run/beamOn " << iNbEventsToSimulate;
    pUImanager->ApplyCommand(hStream.str());
  }
  
  if(bInteractive)
  {
    pUIsession->SessionStart();
    delete pUIsession;
  }
  
  delete pAnalysisManager;
  if(bVisualize) delete pVisManager;
  delete pRunManager;
  
  // merge the output files to one .... events.root
  vector<std::string> auxfiles;
  auxfiles.push_back("geometry.root");
  auxfiles.push_back("physics.root");
  auxfiles.push_back("generator.root");

  fileMerger myMerger("events.root",auxfiles);
  myMerger.Merge();
  myMerger.CleanUp();
	
  return 0;
}

void
usage()
{
  exit(0);
}

