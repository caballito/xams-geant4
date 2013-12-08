#ifndef __DETECTORCONSTRUCTION_H__
#define __DETECTORCONSTRUCTION_H__


#include <globals.hh>

#include <vector>
#include <map>
#include "TFile.h"
#include "TDirectory.h"

using std::vector ;
using std::map ;

class G4Colour ;
class G4LogicalVolume ;
class G4VPhysicalVolume ;
class G4PVReplica ;
class XAMSDetectorMessenger ;

// APC 25-07-2011: for the Klopperboden implementation
#include <G4UnionSolid.hh>

#include <G4UImanager.hh>

#include <G4VUserDetectorConstruction.hh>

class XAMSDetectorConstruction: public G4VUserDetectorConstruction {

	public:
		XAMSDetectorConstruction() ;
		~XAMSDetectorConstruction() ;
  
		G4VPhysicalVolume* Construct() ;

		static G4double GetGeometryParameter(const char *szParameter) ;

		// Function for the messenger interface.
		void SetLiquidLevel(G4double dLiquidLevel) {
			pXeLiquidLevel = dLiquidLevel ;
		}
		void SetNbrCryostatLayers(G4int dNbrCryostatLayers) {
			pNbrCryostatLayers = dNbrCryostatLayers ;
		}

	private:
		void DefineMaterials() ;
		void DefineGeometryParameters() ;
		//
		void ConstructLaboratory() ;
		void ConstructCryostat() ;
		void ConstructInsulationVacuum() ;
		void ConstructChamber() ;
		void ConstructLiquid() ;
		void ConstructTpc() ;
		void ConstructPmt() ;
		void CheckOverlapping();
		void PrintGeometryInformation();
		//
		void MakeDetectorPlots();
		void MakeMaterialPlots();

	private:
		// Logical volumes.
  	G4LogicalVolume* m_pMother_log ;
  	G4LogicalVolume* m_pLab_log ;
		G4LogicalVolume* m_pCryostatOuterWall_log ;
		G4LogicalVolume* m_pInsulationVacuum_log ;
		G4LogicalVolume* m_pCryostatInnerWall_log ;
		G4LogicalVolume* m_pLiq_log ;
		G4LogicalVolume* m_pGas_log ;
		G4LogicalVolume* m_pTeflonTopDisc_log ;
		G4LogicalVolume* m_pTeflonBottomDisc_log ;
		G4LogicalVolume* m_pTeflonCyl_log ;
		G4LogicalVolume* m_pSteelPlate_log ;
		G4LogicalVolume* m_pElectrode_log ;
		G4LogicalVolume* m_pMesh_log ;
		G4LogicalVolume* m_pTpcLiq_log ;
		//G4LogicalVolume* m_pPmt_log ;

  	// Physical volumes.
	  G4VPhysicalVolume* m_pLab_phys ;
  	G4VPhysicalVolume* m_pCryostatOuterWall_phys ;
		G4VPhysicalVolume* m_pInsulationVacuum_phys ;
  	G4VPhysicalVolume* m_pCryostatInnerWall_phys ;
  	G4VPhysicalVolume* m_pLiq_phys ;
  	G4VPhysicalVolume* m_pGas_phys ;
		G4VPhysicalVolume* m_pTeflonTopDisc_phys ;
		G4VPhysicalVolume* m_pTeflonBottomDisc_phys ;
		G4VPhysicalVolume* m_pTeflonCyl_phys ;
		G4VPhysicalVolume* m_pSteelPlate_phys ;
  	G4VPhysicalVolume* m_pElectrode_phys ;
  	G4VPhysicalVolume* m_pMesh_phys ;
  	G4VPhysicalVolume* m_pTpcLiq_phys ;
  	//G4VPhysicalVolume* m_pPmt_phys ;
		//
		// Replica.
		G4PVReplica* m_pElectrode_rep ;
   
	  static map<G4String, G4double> m_hGeometryParameters ;

  	XAMSDetectorMessenger *m_pDetectorMessenger ;

  	// Parameters for communications with XAMSDetectorMessenger.
		G4double pXeLiquidLevel ;
		G4int    pNbrCryostatLayers ;
	  G4String pCryostatType ;
  	G4String pCryostatMaterial ;
  	G4double pCryostatInnerWallThickness ;
  	G4double pCryostatOuterWallThickness ;
  	G4bool   pTPC ;
  	G4bool   pCheckOverlap ;
  	G4UImanager *pUImanager; 
  
  	// ROOT stuff
  	TFile* _fGeom ;
	  TDirectory* _detector ;
  
	  G4double dOuterCryostatMass ;
  	G4double dInnerCryostatMass ; 
  	G4double dTotalCryostatMass ;
} ;

#endif // __DETECTORCONSTRUCTION_H__

