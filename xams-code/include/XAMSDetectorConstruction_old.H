#ifndef __DETECTORCONSTRUCTION_H__
#define __DETECTORCONSTRUCTION_H__


#include <globals.hh>

#include <vector>
#include <map>
#include "TFile.h"
#include "TDirectory.h"

using std::vector;
using std::map;

class G4Colour;
class G4LogicalVolume;
class G4VPhysicalVolume;
class XAMSDetectorMessenger;

// APC 25-07-2011: for the Klopperboden implementation
#include <G4UnionSolid.hh>

#include <G4UImanager.hh>


// #include "Xenon1tPmtSensitiveDetector.hh"

#include <G4VUserDetectorConstruction.hh>

class XAMSDetectorConstruction: public G4VUserDetectorConstruction
{
public:
  XAMSDetectorConstruction();
  ~XAMSDetectorConstruction();
  
  G4VPhysicalVolume* Construct();
	
  static G4double GetGeometryParameter(const char *szParameter);


private:
  void DefineMaterials();
  void DefineGeometryParameters();

  void ConstructLaboratory();
  void ConstructCrystal();
  void CheckOverlapping();
  void PrintGeometryInformation();
  //
  void MakeDetectorPlots();
  void MakeMaterialPlots();

private:
  // logical volumes
  G4LogicalVolume *m_pMotherLogicalVolume;
  G4LogicalVolume *m_pLabLogicalVolume;

  G4LogicalVolume *Xtal_LogicalVolume;
  // physical volumes
  G4VPhysicalVolume *m_pLabPhysicalVolume;
  G4VPhysicalVolume *Xtal_PhysicalVolume;
   
  static map<G4String, G4double> m_hGeometryParameters;
	
  XAMSDetectorMessenger *m_pDetectorMessenger;

//  Xenon1tPmtSensitiveDetector *pPmtSD;

  // parameters for communications with XAMSDetectorMessenger
  G4String pCryostatType;
  G4String pCryostatMaterial;
  G4double pCryostatInnerWallThickness;
  G4double pCryostatOuterWallThickness;
  G4bool   pTPC;
  G4bool   pCheckOverlap;
  //G4UImanager *pUImanager; 
  
  // ROOT stuff
  TFile      *_fGeom;
  TDirectory *_detector;
  
  G4double dOuterCryostatMass;
  G4double dInnerCryostatMass; 
  G4double dTotalCryostatMass;
};

#endif // __DETECTORCONSTRUCTION_H__

