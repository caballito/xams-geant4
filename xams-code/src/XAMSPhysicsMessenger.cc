//
// Communications with the PhysicsList class
//
// A.P. Colijn 27-07-2011
//

#include <G4Geantino.hh>
#include <G4ThreeVector.hh>
#include <G4ParticleTable.hh>
#include <G4UIdirectory.hh>
#include <G4UIcmdWithoutParameter.hh>
#include <G4UIcmdWithAString.hh>
#include <G4UIcmdWithADoubleAndUnit.hh>
#include <G4UIcmdWith3Vector.hh>
#include <G4UIcmdWith3VectorAndUnit.hh>
#include <G4UIcmdWithAnInteger.hh>
#include <G4UIcmdWithADouble.hh>
#include <G4UIcmdWithABool.hh>
#include <G4Tokenizer.hh>
#include <G4ios.hh>
#include <fstream>
#include <iomanip>

#include "XAMSPhysicsMessenger.hh"
#include "XAMSPhysicsList.hh"

XAMSPhysicsMessenger::XAMSPhysicsMessenger(XAMSPhysicsList *pPhysicsList):
  m_pPhysicsList(pPhysicsList)
{
  // Create directory.
  m_pDirectory = new G4UIdirectory("/run/physics/");
  m_pDirectory->SetGuidance("PhysicsList control commands.");

  // Set EM model.
  m_pEMlowEnergyModelCmd = new G4UIcmdWithAString("/run/physics/setEMlowEnergyModel", this);
  m_pEMlowEnergyModelCmd->SetGuidance("Choose low energy EM model from:");
  m_pEMlowEnergyModelCmd->SetGuidance("<emstandard = no low E> <emlivermore> <empenelope> <old>");
  m_pEMlowEnergyModelCmd->SetDefaultValue("emlivermore");
  m_pEMlowEnergyModelCmd->AvailableForStates(G4State_PreInit);

  // Set Hadronic model.
  m_pHadronicModelCmd = new G4UIcmdWithAString("/run/physics/setHadronicModel", this);
  m_pHadronicModelCmd->SetGuidance("Choose hadronic model from:");
  m_pHadronicModelCmd->SetGuidance("<custom = standard Xenon100> <QGSP_BERT_HP>");
  m_pHadronicModelCmd->SetDefaultValue("custom");
  m_pHadronicModelCmd->AvailableForStates(G4State_PreInit);
	
  // Switch Cerenkov radiation on - or - off.
  m_pCerenkovCmd = new G4UIcmdWithABool("/run/physics/setCerenkov", this);
  m_pCerenkovCmd->SetGuidance("Switch Cerenkov radiation on (=true) or off (=false)");
  m_pCerenkovCmd->SetDefaultValue(true);
  m_pCerenkovCmd->AvailableForStates(G4State_PreInit);
	
	// Set tracking cut.
	m_pTrackingCutCmd = new G4UIcmdWithADoubleAndUnit("/run/physics/setTrackingCut",this) ;
	m_pTrackingCutCmd->SetGuidance("Set the range cut for tracking secondaries.") ;
	m_pTrackingCutCmd->SetGuidance("The default range cut is 1. mm.") ;
	m_pTrackingCutCmd->SetParameterName("trackingCut",false) ;
	m_pTrackingCutCmd->SetDefaultValue(1.) ;
	m_pTrackingCutCmd->SetUnitCategory("Length") ;
	m_pTrackingCutCmd->SetDefaultUnit("mm") ;
	m_pTrackingCutCmd->AvailableForStates(G4State_PreInit) ;

	// Set production cut.
	m_pProductionCutCmd = new G4UIcmdWithADoubleAndUnit("/run/physics/setProductionCut",this) ;
	m_pProductionCutCmd->SetGuidance("Set the energy threshold for secondaries production.") ;
	m_pProductionCutCmd->SetGuidance("The default production cut is 100. eV.") ;
	m_pProductionCutCmd->SetParameterName("productionCut",false) ;
	m_pProductionCutCmd->SetDefaultValue(100.) ;
	m_pProductionCutCmd->SetUnitCategory("Energy") ;
	m_pProductionCutCmd->SetDefaultUnit("eV") ;
	m_pProductionCutCmd->AvailableForStates(G4State_PreInit) ;

  // Set the defaults.

  m_pPhysicsList->SetEMlowEnergyModel("emlivermore");
  m_pPhysicsList->SetCerenkov(false);
	m_pPhysicsList->SetTrackingCut(1.) ;
	m_pPhysicsList->SetProductionCut(100.) ;
	
}

XAMSPhysicsMessenger::~XAMSPhysicsMessenger()
{
  delete m_pDirectory;
}

void XAMSPhysicsMessenger::SetNewValue(G4UIcommand* pUIcommand, G4String newValues)
{
  if( pUIcommand == m_pEMlowEnergyModelCmd )
    m_pPhysicsList->SetEMlowEnergyModel(newValues);

  if( pUIcommand == m_pHadronicModelCmd )
    m_pPhysicsList->SetHadronicModel(newValues);
	
  if( pUIcommand == m_pCerenkovCmd )
    m_pPhysicsList->SetCerenkov(m_pCerenkovCmd->GetNewBoolValue(newValues));
	
	if( pUIcommand == m_pTrackingCutCmd )
		m_pPhysicsList->SetTrackingCut(m_pTrackingCutCmd->GetNewDoubleValue(newValues)) ;

	if( pUIcommand == m_pProductionCutCmd )
		m_pPhysicsList->SetProductionCut(m_pProductionCutCmd->GetNewDoubleValue(newValues)) ;
}

