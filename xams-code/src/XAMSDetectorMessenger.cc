#include <G4ThreeVector.hh>
#include <G4RotationMatrix.hh>
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

#include "XAMSDetectorMessenger.hh"
#include "XAMSDetectorConstruction.hh"

XAMSDetectorMessenger::XAMSDetectorMessenger(XAMSDetectorConstruction *pXeDetector) 
:m_pXeDetector(pXeDetector)
{ 
	m_pDetectorDir = new G4UIdirectory("/Xe/detector/");
	m_pDetectorDir->SetGuidance("detector control.");

	// Set the liquid Xe level in the test chamber.
	m_pLiquidLevelCmd = new G4UIcmdWithADoubleAndUnit("/Xe/detector/setLiquidLevel",this) ;
	m_pLiquidLevelCmd->SetGuidance("Give the liquid Xe level in the test chamber.") ;
	m_pLiquidLevelCmd->SetGuidance("The chamber height is 330. * mm. The level must not exceed this!") ;
	m_pLiquidLevelCmd->SetGuidance("The default value is 300.0 * mm.") ;
	m_pLiquidLevelCmd->SetParameterName("liquidLevel",false) ;
	m_pLiquidLevelCmd->SetDefaultValue(300.) ;
	m_pLiquidLevelCmd->SetUnitCategory("Length") ;
	m_pLiquidLevelCmd->SetDefaultUnit("mm") ;
	m_pLiquidLevelCmd->AvailableForStates(G4State_PreInit) ;
	
	// Set the number of cryostat layers arounf the lXe volume.
	m_pNbrCryostatLayersCmd = new G4UIcmdWithAnInteger("/Xe/detector/setNbrCryostatLayers",this) ;
	m_pNbrCryostatLayersCmd->SetGuidance("Specify the number of cryostat layers around the lXe volume.") ;
	m_pNbrCryostatLayersCmd->SetGuidance("The default is none (so the lXe only).") ;
	m_pNbrCryostatLayersCmd->SetParameterName("nbrCryostatLayers",false) ;
	m_pNbrCryostatLayersCmd->SetDefaultValue(0) ;
	m_pNbrCryostatLayersCmd->AvailableForStates(G4State_PreInit) ;
	
	// Set default values.
	m_pXeDetector->SetLiquidLevel(300.) ;
	m_pXeDetector->SetNbrCryostatLayers(0) ;
}

XAMSDetectorMessenger::~XAMSDetectorMessenger()
{
  delete m_pDetectorDir;
	//delete m_pLiquidLevelCmd ;
}

void XAMSDetectorMessenger::SetNewValue(G4UIcommand *pUIcommand, G4String hNewValue)
{
	if( pUIcommand == m_pLiquidLevelCmd )
		m_pXeDetector->SetLiquidLevel(m_pLiquidLevelCmd->GetNewDoubleValue(hNewValue)) ;

	if( pUIcommand == m_pNbrCryostatLayersCmd )
		m_pXeDetector->SetNbrCryostatLayers(m_pNbrCryostatLayersCmd->GetNewIntValue(hNewValue)) ;
}

//G4String XAMSDetectorMessenger::GetCurrentValue(G4UIcommand *pUIcommand)
//{
//  G4String cv;
//
//	return cv;
//}
		
