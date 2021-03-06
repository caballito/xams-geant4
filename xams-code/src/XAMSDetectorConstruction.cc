#include <G4Material.hh>
#include <G4NistManager.hh>
#include <G4Box.hh>
#include <G4Tubs.hh>
#include <G4Sphere.hh>
#include <G4Torus.hh>
//#include <G4Orb.hh>
#include <G4Polyhedra.hh>
#include <G4Polycone.hh>
#include <G4Ellipsoid.hh>
//#include <G4Trd.hh>
//#include <G4Cons.hh>
#include <G4UnionSolid.hh>
//#include <G4IntersectionSolid.hh>
#include <G4SubtractionSolid.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4PVParameterised.hh>
#include <G4OpBoundaryProcess.hh>
#include <G4SDManager.hh>
#include <G4ThreeVector.hh>
#include <G4RotationMatrix.hh>
#include <G4VisAttributes.hh>
#include <G4Colour.hh>
#include <globals.hh>

#include <vector>
#include <numeric>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <cassert>

using std::vector ;
using std::stringstream ;
using std::max ;
 
#include "XAMSDetectorMessenger.hh"

#include "XAMSSensitiveDetector.hh"
#include "XAMSDetectorConstruction.hh"

#include "TFile.h"
#include "TParameter.h"

std::map<G4String, G4double> XAMSDetectorConstruction::m_hGeometryParameters ;
 
XAMSDetectorConstruction::XAMSDetectorConstruction() {
  m_pDetectorMessenger = new XAMSDetectorMessenger(this) ;
}

XAMSDetectorConstruction::~XAMSDetectorConstruction() {
  delete m_pDetectorMessenger;
}

G4VPhysicalVolume* XAMSDetectorConstruction::Construct() {
  // Enter main detector construction routine.
  DefineMaterials() ;
  //
  DefineGeometryParameters() ;
  //  
  ConstructLaboratory() ;
	//
	// Check how many layers we have to construct (parameter from messenger).
	G4cout << "++ Required number of cryostat layers around the liquid Xe volume: " 
		<< pNbrCryostatLayers << G4endl
		<< "   (0: none, 1: gas layer and inner wall, 2: also insulation vacuum, 3: also outer wall)." 
		<< G4endl ;
	//
	if (pNbrCryostatLayers > 2) {
		G4cout << "++ Constructing the outer wall of the cryostat..." ;
		ConstructCryostat() ;
		G4cout << " Done." << G4endl ;
	}
	//
	if (pNbrCryostatLayers > 1) {
		G4cout << "++ Constructing the insulation vacuum..." ;
		ConstructInsulationVacuum() ;
		G4cout << " Done." << G4endl ;
	}
	//
	if (pNbrCryostatLayers > 0) {
		G4cout << "++ Constructing the inner wall of the cryostat and the Xe gas layer..." ;
		ConstructChamber() ;
		G4cout << " Done." << G4endl ;
	}
	//
	G4cout << "++ Constructing the liquid Xe volume..." ;
	ConstructLiquid() ;
	G4cout << " Done." << G4endl ;
	//
	//if (pNbrCryostatLayers > 0){
		G4cout << "++ Constructing the TPC..." ;
		ConstructTpc() ;
		G4cout << " Done." << G4endl ;
	//}
    
  // Info on our geometry.
  //PrintGeometryInformation() ;
   
  // Write info about geometry into root output.
  MakeDetectorPlots() ;
  
  return m_pLab_phys ;
}

void XAMSDetectorConstruction::DefineMaterials() {
  G4NistManager* pNistManager = G4NistManager::Instance() ;
  
  //=============== Elements =====================
  G4Element* H  = new G4Element("Hydrogen", "H",  1.,  1.0079 * g/mole) ;
  G4Element* N  = new G4Element("Nitrogen", "N",  7.,  14.007 * g/mole) ;
  G4Element* O  = new G4Element("Oxygen",   "O",  8.,  15.999 * g/mole) ;
  G4Element* Fe = pNistManager->FindOrBuildElement("Fe") ;
  G4Element* C  = pNistManager->FindOrBuildElement("C") ;
  G4Element* Cr = pNistManager->FindOrBuildElement("Cr") ;
  G4Element* Ni = pNistManager->FindOrBuildElement("Ni") ;
	// Natural Xe isotope composition as in NEST README.
	/*G4Element* natXe = new G4Element("natural Xe","natXe",9) ;
	natXe->AddIsotope(Xe124, 0.09 * perCent ) ;
	natXe->AddIsotope(Xe126, 0.09 * perCent ) ;
	natXe->AddIsotope(Xe128, 1.92 * perCent ) ;
	natXe->AddIsotope(Xe129, 26.44* perCent ) ;
	natXe->AddIsotope(Xe130, 4.08 * perCent ) ;
	natXe->AddIsotope(Xe131, 21.18* perCent ) ;
	natXe->AddIsotope(Xe132, 26.89* perCent ) ;
	natXe->AddIsotope(Xe134, 10.44* perCent ) ;
	natXe->AddIsotope(Xe136, 8.87 * perCent ) ;
	*/

  //=============== Materials ==================== 
  
  //----- Air ----------------
  pNistManager->FindOrBuildMaterial("G4_AIR") ;
  
	//----- Xenon -------
  pNistManager->FindOrBuildMaterial("G4_Xe") ;
  pNistManager->FindOrBuildMaterial("G4_lXe") ;
	// Alternatively the correct detailed isotope mixture.
	//G4Material* liquidXe = new G4Material("liquidXe",2.953*g/cm3, 1) ;
	//liquidXe->AddElement(natXe,1) ;
  
	//----- liquid Argon -------
  pNistManager->FindOrBuildMaterial("G4_lAr") ;
  
	//----- Copper -------------
	pNistManager->FindOrBuildMaterial("G4_Cu") ;

	//----- Teflon -------------
	pNistManager->FindOrBuildMaterial("G4_TEFLON") ;

  //----- Vacuum -------------
  G4Material* Vacuum = new G4Material("Vacuum", 1.e-20*g/cm3, 2, kStateGas) ;
  Vacuum->AddElement(N, 0.755) ;
  Vacuum->AddElement(O, 0.245) ;
  
  //----- Water --------------
  G4Material* Water = new G4Material("Water", 1.*g/cm3, 2, kStateLiquid) ;
  Water->AddElement(H, 2) ;
  Water->AddElement(O, 1) ;

  //----- Stainles steel -----
  G4Material* Steel = new G4Material("Steel", 7.95*g/cm3, 4, kStateSolid) ;	// TODO: Find exact density or subsitute with G4_STAINLESS_STEEL.
	Steel->AddElement(Fe, 0.7195) ;	// Took stainless steel 304 = X5CrNi18-10.
  Steel->AddElement(C , 0.0005) ;
  Steel->AddElement(Cr, 0.18) ;
  Steel->AddElement(Ni, 0.10) ;

}

void XAMSDetectorConstruction::DefineGeometryParameters() {
	// Fix all dimensions here to use them inside the single
	// Construct...() methods.
	
	// World dimensions.
  m_hGeometryParameters["labHeight"] = 2.0 * m ;	// Total height of the world (not half Z!).
  m_hGeometryParameters["labRadius"] = 1.0 * m ;
	
	// TODO: Get precise measures from drawings.
	// Cryostat outer wall dimensions.
  m_hGeometryParameters["outerWallHeight"] = 501.0 * mm ;	// Total height.
	m_hGeometryParameters["outerWallThickness"] = 5.0 * mm ;
	
	// Insulation vacuum dimensions.
  m_hGeometryParameters["insulationHeight"] = 480.0 * mm ;	// Total height.
  m_hGeometryParameters["insulationThickness"] = 50.3 * mm ;
	
	// Cryostat inner wall dimensions.
  m_hGeometryParameters["innerWallHeight"] = 380.0 * mm ;	// Total height.
  m_hGeometryParameters["innerWallThickness"] = 5.0 * mm ;
	
	// Chamber dimensions. The inside of the chamber of the inner
	// vessel is 340 mm high, thus limiting the liquid level.
	// The difference between the set liquid level and 340 mm 
	// will be assigned to the gaseous gap.
	const G4double pChamberHeight = 340. * mm ;	// Total height (not halfZ!).
	m_hGeometryParameters["chamberHeight"] = pChamberHeight ;
  m_hGeometryParameters["XeRadius"] = 76.2 * mm ;
	
	// Liquid Xe height.
	// If liquid Xe surrounded by other layers use dimensions from the messenger.
	if (pNbrCryostatLayers > 0){
		if(pXeLiquidLevel < pChamberHeight)
			m_hGeometryParameters["liquidLevel"] = pXeLiquidLevel ;
		else
			// Use epsilon of 0.1 mm because G4 doesn't like volumes of 0 height (for gas gap).
			m_hGeometryParameters["liquidLevel"] = pChamberHeight - 0.1 * mm ;
	}
	else {	// If it's the only volume inside the world.
		m_hGeometryParameters["liquidLevel"] = 500. * mm ;
	}

	// Gaseous Xe layer height; total, not halfZ.
  m_hGeometryParameters["gXeHeight"] = pChamberHeight - m_hGeometryParameters["liquidLevel"] ;
}

G4double XAMSDetectorConstruction::GetGeometryParameter(const char *szParameter) {
  return m_hGeometryParameters[szParameter] ;
}

//================= Laboratory ===================
void XAMSDetectorConstruction::ConstructLaboratory() {
	// Dimensions.
  const G4double dLabHalfZ = 0.5*GetGeometryParameter("labHeight") ;
  const G4double dLabRadius = GetGeometryParameter("labRadius") ;
  
	// Material.
  G4Material* Vacuum = G4Material::GetMaterial("Vacuum") ;
  
	// Volumes.
  G4Tubs* pLab_tubs = new G4Tubs("Lab_tubs", 0.*cm,
			dLabRadius, dLabHalfZ, 0.*deg, 360.*deg) ;
  m_pLab_log = new G4LogicalVolume(pLab_tubs, Vacuum, "LabVolume") ;
  m_pLab_phys = new G4PVPlacement(0, G4ThreeVector(), m_pLab_log,
			"Lab", 0, false, 0) ;
  m_pMother_log = m_pLab_log ;
  
  m_pLab_log->SetVisAttributes(G4VisAttributes::Invisible) ;	// Make world invisible.
}

//================= Cryostat =====================
void XAMSDetectorConstruction::ConstructCryostat() {
	// Dimensions.
	const G4double dCryostatOuterWallHalfZ = 
		0.5 * GetGeometryParameter("outerWallHeight") ;
	const G4double dCryostatOuterWallRadius = ( 
			GetGeometryParameter("XeRadius") 
			+ GetGeometryParameter("innerWallThickness") 
			+ GetGeometryParameter("insulationThickness")
			+ GetGeometryParameter("outerWallThickness") ) ;
	
	// Position.
	const G4double pCryostatOuterWallZ = -10.*cm ;
	
	// Material.
  G4Material* Steel = G4Material::GetMaterial("Steel") ;
	
	// Volumes.
	G4Tubs* pCryostatOuterWall_tubs = new G4Tubs("CryostatOuterWall_tubs",
			0.*cm,dCryostatOuterWallRadius,dCryostatOuterWallHalfZ,0.*deg,360.*deg) ;
	m_pCryostatOuterWall_log = new G4LogicalVolume(pCryostatOuterWall_tubs,
			Steel,"CryostatOuterWallVolume") ;
	m_pCryostatOuterWall_phys = new G4PVPlacement(0,
			G4ThreeVector(0.*cm,0.*cm,pCryostatOuterWallZ),
			m_pCryostatOuterWall_log,"CryostatWall",m_pLab_log,false,0) ;

	// Visibility attributes.
	G4Colour hCryostatColor(1.,0.4,0,0) ;	// (red,green,blue,alpha=opacity).
	G4VisAttributes* pCryostatVisAtt = new G4VisAttributes(hCryostatColor) ;
	pCryostatVisAtt->SetVisibility(true) ;
	m_pCryostatOuterWall_log->SetVisAttributes(pCryostatVisAtt) ;
}

//================= Insulation vacuum ============
void XAMSDetectorConstruction::ConstructInsulationVacuum() {
	// Dimensions.
  const G4double dInsulationVacuumHalfZ = 0.5 * GetGeometryParameter("insulationHeight") ;
  const G4double dInsulationVacuumRadius = ( 
			GetGeometryParameter("XeRadius") 
			+ GetGeometryParameter("innerWallThickness") 
			+ GetGeometryParameter("insulationThickness") ) ;

	// Position not needed: centred in either world or cryostat.
	
	// Material.
  G4Material* Vacuum = G4Material::GetMaterial("Vacuum") ;

	// Volumes.
	G4Tubs* pInsulationVacuum_tubs = new G4Tubs("InsulationVacuum_tubs",0.*cm,
			dInsulationVacuumRadius,dInsulationVacuumHalfZ,0.*deg,360.*deg) ;
	m_pInsulationVacuum_log = new G4LogicalVolume(pInsulationVacuum_tubs,Vacuum,
			"InsulationVacuumVolume") ;
	// Put inside CryostatOuterWallVolume or world.
	if ( pNbrCryostatLayers > 2 )
		m_pInsulationVacuum_phys = new G4PVPlacement(0,G4ThreeVector(),
				m_pInsulationVacuum_log,"InsulationVacuum",m_pCryostatOuterWall_log,false,0) ;
	else
		m_pInsulationVacuum_phys = new G4PVPlacement(0,G4ThreeVector(),
				m_pInsulationVacuum_log,"InsulationVacuum",m_pLab_log,false,0) ;
	
}

//================= Chamber ======================
// The test chamber has an outer shell (CryostatInnerWall) of stainless steel.
// The inner volume is split into a liquid Xe volume (not in this method)
// and a gaseous Xe volume (Gas).
void XAMSDetectorConstruction::ConstructChamber() {
	// Dimensions.
	const G4double dCryostatInnerWallHalfZ = 
		0.5 * GetGeometryParameter("innerWallHeight") ;
	const G4double dCryostatInnerWallRadius = ( 
			GetGeometryParameter("XeRadius")
			+ GetGeometryParameter("innerWallThickness") ) ;
	//
  const G4double dGasHalfZ = 0.5 * GetGeometryParameter("chamberHeight") ;
	const G4double dGasRadius = GetGeometryParameter("XeRadius") ;
  //const G4double dLiqHalfZ = 0.5 * GetGeometryParameter("liquidLevel") ;
	
	// Positions.
	const G4double pCryostatInnerWallZ = -26.* mm ;
	//const G4double pGasZ = dLiqHalfZ ;

	// Materials.
  G4Material* Steel = G4Material::GetMaterial("Steel") ;
  G4Material* Xe    = G4Material::GetMaterial("G4_Xe") ;

	// Volumes.
	G4Tubs* pCryostatInnerWall_tubs = new G4Tubs("CryostatInnerWall_tubs",
			0.*cm,dCryostatInnerWallRadius,dCryostatInnerWallHalfZ,0.*deg,360.*deg) ;
	m_pCryostatInnerWall_log = new G4LogicalVolume(pCryostatInnerWall_tubs,Steel,
			"CryostatInnerWallVolume") ;
	if ( pNbrCryostatLayers > 1 )
		m_pCryostatInnerWall_phys = new G4PVPlacement(0,
				G4ThreeVector(0.*cm,0.*cm,pCryostatInnerWallZ),m_pCryostatInnerWall_log,
				"CryostatInnerWall",m_pInsulationVacuum_log,false,0) ;
	else
		m_pCryostatInnerWall_phys = new G4PVPlacement(0,
				G4ThreeVector(),m_pCryostatInnerWall_log,
				"CryostatInnerWall",m_pLab_log,false,0) ;
	//
	G4Tubs* pGas_tubs = new G4Tubs("Gas_tubs",0.*cm,
			dGasRadius,dGasHalfZ,0.*deg,360.*deg) ;
	m_pGas_log = new G4LogicalVolume(pGas_tubs,Xe,"GasVolume") ;
	m_pGas_phys = new G4PVPlacement(0,
			G4ThreeVector(),m_pGas_log,
			"GaseousXenon",m_pCryostatInnerWall_log,false,0) ;
  
	// Visibility attributes.
	G4Colour hGasColor(0,.8,0.8,.5) ;
	G4VisAttributes* pGasVisAtt = new G4VisAttributes(hGasColor) ;
	pGasVisAtt->SetVisibility(true) ;
	m_pGas_log->SetVisAttributes(pGasVisAtt) ;
}

//================= Liquid xenon =================
void XAMSDetectorConstruction::ConstructLiquid() {
	// Dimensions.
	const G4double dXeDiscHalfZ = 10. * mm ;
  const G4double dXeCylHalfZ = 0.5 * GetGeometryParameter("liquidLevel") - dXeDiscHalfZ ;
  const G4double dXeOuterRadius = GetGeometryParameter("XeRadius") ;
	const G4double dXeInnerRadius = 74. * mm ;

	// Position in case of enclosing volumes; otherwise centred in world.
	const G4double dXeDiscZ = -0.5 * GetGeometryParameter("chamberHeight")
		+ dXeDiscHalfZ ;
	const G4double dXeCylZ = -0.5 * ( GetGeometryParameter("chamberHeight")
			- GetGeometryParameter("liquidLevel")) + dXeDiscHalfZ ;
	
	// Material.
  G4Material* lXe = G4Material::GetMaterial("G4_lXe") ;
  //G4Material* lXe = G4Material::GetMaterial("liquidXe") ;

	// Volumes.
	G4Tubs* pLiq_disc = new G4Tubs("Liq_disc",0.*cm,
			dXeOuterRadius,dXeDiscHalfZ,0.*deg,360.*deg) ;
	m_pXeDisc_log = new G4LogicalVolume(pLiq_disc,lXe,"LiquidXeDisc") ;
	//
	G4Tubs* pLiq_cyl = new G4Tubs("Liq_cyl",dXeInnerRadius,
			dXeOuterRadius,dXeCylHalfZ,0.*deg,360.*deg) ;
	m_pXeCyl_log = new G4LogicalVolume(pLiq_cyl,lXe,"LiquidXeCyl") ;
	//
	//if ( pNbrCryostatLayers > 0 ){
		m_pXeDisc_phys = new G4PVPlacement(0,
				G4ThreeVector(0.*cm,0.*cm,dXeDiscZ),m_pXeDisc_log,
				"LiquidXenonBottom",m_pGas_log,false,0) ;
		m_pXeCyl_phys = new G4PVPlacement(0,
				G4ThreeVector(0.*mm,0.*mm,dXeCylZ),m_pXeCyl_log,
				"LiquidXenonAround",m_pGas_log,false,0) ;
	/*}
	else {
		m_pXeDisc_phys = new G4PVPlacement(0,
				G4ThreeVector(),m_pXeDisc_log,
				"LiquidXenon",m_pLab_log,false,0) ;
	}*/
/*
  // Make the lXe the sensitive detector.
  G4SDManager* pSDManager = G4SDManager::GetSDMpointer() ;
  XAMSSensitiveDetector* pLiq_SD = new XAMSSensitiveDetector("XAMS/lXe") ;
  pSDManager->AddNewDetector(pLiq_SD) ;
  m_pLiq_log->SetSensitiveDetector(pLiq_SD) ;
	*/
	// Visibility attributes.
	G4Colour hLiqColor(0,0.4,1.,0) ;
	G4VisAttributes* pLiqVisAtt = new G4VisAttributes(hLiqColor) ;
	pLiqVisAtt->SetVisibility(true) ;
	m_pXeDisc_log->SetVisAttributes(pLiqVisAtt) ;
	m_pXeCyl_log->SetVisAttributes(pLiqVisAtt) ;
}

//================= TPC ==========================
void XAMSDetectorConstruction::ConstructTpc() {
	// Dimensions.
	const G4double dBottomXeHalfZ = 10. * mm ;
	const G4double dSteelPlateHalfZ = 1. * mm ;
	//
	// Drift length of 8 * 12.5 mm = 100 mm.
	const G4double dTeflonCylHalfZ = 50. * mm ;
	const G4double dTeflonDiscHalfZ = 40. * mm ;
	const G4double dTeflonOuterRadius = 74. * mm ;
	const G4double dTeflonInnerRadius = 30. * mm ;
	//
	G4double dXeHalfZ ;
	const G4double dLevel = 0.5 * GetGeometryParameter("liquidLevel") - dBottomXeHalfZ
		- dSteelPlateHalfZ - dTeflonDiscHalfZ ;
	if ( dLevel < dTeflonCylHalfZ )
		dXeHalfZ = dLevel ;
	else 
		dXeHalfZ = dTeflonCylHalfZ ;
	//
	const G4double dElectrodeHalfZ = 1. * mm ;
	const G4double dElectrodeOuterRadius = 55. * mm ;
	const G4double dElectrodeInnerRadius = 45. * mm ;
	//
	const G4double dMeshHalfZ = .8 * mm ;
	const G4double dMeshRadius = 55. * mm ;

	// Positions.
	const G4double dSteelPlateZ = -0.5 * GetGeometryParameter("chamberHeight") 
		+ 2. * dBottomXeHalfZ + dSteelPlateHalfZ ;
	const G4double dTeflonBottomDiscZ = -0.5 * GetGeometryParameter("chamberHeight")
		+ 2. * dBottomXeHalfZ + 2. * dSteelPlateHalfZ + dTeflonDiscHalfZ ; 
	const G4double dTeflonCylZ = -0.5 * GetGeometryParameter("chamberHeight")
		+ 2. * dBottomXeHalfZ + 2. * dSteelPlateHalfZ + 2. * dTeflonDiscHalfZ
		+ dTeflonCylHalfZ ;
	const G4double dTeflonTopDiscZ = -0.5 * GetGeometryParameter("chamberHeight")
		+ 2. * dBottomXeHalfZ + 2. * dSteelPlateHalfZ + 3. * dTeflonDiscHalfZ
		+ 2. * dTeflonCylHalfZ ;
	const G4double dXeZ = -0.5 * GetGeometryParameter("chamberHeight")
		+ 2. * dBottomXeHalfZ + 2. * dSteelPlateHalfZ + 2. * dTeflonDiscHalfZ
		+ dXeHalfZ ;

	// Material.
	G4Material* Teflon = G4Material::GetMaterial("G4_TEFLON") ;
	G4Material* Cu = G4Material::GetMaterial("G4_Cu") ;
	G4Material* Steel = G4Material::GetMaterial("Steel") ;
	G4Material* lXe = G4Material::GetMaterial("G4_lXe") ;

	// Volumes.
	// Bottom steel plate.
	G4Tubs* pSteelPlate = new G4Tubs("Steel_plate",0.*cm,
			dTeflonOuterRadius,1.*mm,0.*deg,360.*deg) ;
	m_pSteelPlate_log = new G4LogicalVolume(pSteelPlate,Steel,"SteelPlate") ;
	m_pSteelPlate_phys = new G4PVPlacement(0,
			G4ThreeVector(0.*mm,0.*mm,dSteelPlateZ),m_pSteelPlate_log,"BottomSteelPlate",
			m_pGas_log,false,0) ;
	// Teflon.
	G4Tubs* pTeflonDisc = new G4Tubs("Teflon_disc",0.*cm,
			dTeflonOuterRadius,dTeflonDiscHalfZ,0.*deg,360.*deg) ;
	m_pTeflonTopDisc_log = new G4LogicalVolume(pTeflonDisc,Teflon,
			"TeflonTopDisc") ;
	m_pTeflonTopDisc_phys = new G4PVPlacement(0,
			G4ThreeVector(0.*mm,0.*mm,dTeflonTopDiscZ),m_pTeflonTopDisc_log,
			"TopTeflonDisc",m_pGas_log,false,0) ;
	//
	m_pTeflonBottomDisc_log = new G4LogicalVolume(pTeflonDisc,Teflon,
			"TeflonBottomDisc") ;
	m_pTeflonBottomDisc_phys = new G4PVPlacement(0,
			G4ThreeVector(0.*mm,0.*mm,dTeflonBottomDiscZ),m_pTeflonBottomDisc_log,
			"BottomTeflonDisc",m_pGas_log,false,0) ;
	//
	G4Tubs* pTeflonCyl = new G4Tubs("Teflon_tubs",dTeflonInnerRadius,
			dTeflonOuterRadius,dTeflonCylHalfZ,0.*deg,360.*deg) ;
	m_pTeflonCyl_log = new G4LogicalVolume(pTeflonCyl,Teflon,"TeflonCylinder") ;
	m_pTeflonCyl_phys = new G4PVPlacement(0,
			G4ThreeVector(0.*mm,0.*mm,dTeflonCylZ),m_pTeflonCyl_log,
			"TeflonCylinder",m_pGas_log,false,0) ;
	//
	// Electrode(s).
	G4Tubs* pElectrode = new G4Tubs("Cu_tubs",dElectrodeInnerRadius,
			dElectrodeOuterRadius,dElectrodeHalfZ,0.*deg,360.*deg) ;
	m_pElectrode_log = new G4LogicalVolume(pElectrode,Cu,"ElectrodeRing") ;
	// TODO: Multiply and place evenly over drift length.
	/*m_pElectrode_phys = new G4PVPlacement(0,G4ThreeVector(),
			m_pElectrode_log,"CuRing",m_pTeflonCyl_log,false,6) ;*/
	// Replicas.
	/*m_pElectrode_rep = new G4PVReplica("CuRep",	// Name.
			m_pElectrode_log,	// Logical volume.
			m_pTeflon_log,	// Mother volume.
			kXAxis,	// Axis on which the copies are shifted.
			6,	// Number of replicated volumes.
			, // Widtch of single replicated volume along axis.
			0.*mm	// Offset associated to mother offset along axis.
			) ;*/
	//
	// Mesh(es).
	G4Tubs* pMesh = new G4Tubs("Steel_tubs",0.*cm,
			dMeshRadius,dMeshHalfZ,0.*deg,360.*deg) ;
	m_pMesh_log = new G4LogicalVolume(pMesh,Steel,"SteelMesh") ;
	// TODO: put at correct height, double and divide into lXe.
	/*m_pMesh_phys = new G4PVPlacement(0,
			G4ThreeVector(0.*cm,0.*cm,-1.*dTeflonCylHalfZ - 5.*mm),
			m_pMesh_log,"Mesh",m_pTeflonCyl_log,false,0) ;*/
	//
	// Inner lXe.
	G4Tubs* pTpcLiq_tubs = new G4Tubs("Liq_tubs",0.*cm,
			dTeflonInnerRadius,dXeHalfZ,0.*deg,360.*deg) ;
	m_pTpcLiq_log = new G4LogicalVolume(pTpcLiq_tubs,lXe,"InnerLiquidVolume") ;
	m_pTpcLiq_phys = new G4PVPlacement(0,G4ThreeVector(0.*mm,0.*mm,dXeZ),
			m_pTpcLiq_log,"InnerLiquidXenon",m_pGas_log,false,0) ;

  // Make the lXe the sensitive detector.
  G4SDManager* pSDManager = G4SDManager::GetSDMpointer() ;
  XAMSSensitiveDetector* pTpcLiq_SD = new XAMSSensitiveDetector("XAMS/lXe") ;
  pSDManager->AddNewDetector(pTpcLiq_SD) ;
  m_pTpcLiq_log->SetSensitiveDetector(pTpcLiq_SD) ;
	// Visibility attributes.
	G4Colour hLiqColor(0,0.4,1.,0) ;
	G4VisAttributes* pLiqVisAtt = new G4VisAttributes(hLiqColor) ;
	pLiqVisAtt->SetVisibility(true) ;
	m_pTpcLiq_log->SetVisAttributes(pLiqVisAtt) ;

}

//================= PMT ==========================
void XAMSDetectorConstruction::ConstructPmt() {
	// Nothing yet.

}


void XAMSDetectorConstruction::PrintGeometryInformation() {
	// Nothing here.
}

void XAMSDetectorConstruction::MakeDetectorPlots() {
  _fGeom = new TFile("geometry.root","RECREATE") ;
  _detector = _fGeom->mkdir("detector") ;

  // Materials.
  MakeMaterialPlots() ;
  //
  _fGeom->Write() ;
  _fGeom->Close() ;
}

void XAMSDetectorConstruction::MakeMaterialPlots() {
  // Make a list of materials for graphs.
  G4int nmaterial = G4Material::GetNumberOfMaterials() ;
  G4cout << "MakeDetectorPlots:: Number of materials = " << nmaterial << G4endl ;

  TDirectory* _materials = _detector->mkdir("materials") ;
  _materials->cd() ;

  //  for(G4int imat=0; imat<(G4int)matNames.size(); imat++){
  vector<TDirectory*> matdirs;
  
  for(G4int imat=0; imat<nmaterial; imat++) {
    G4Material *mat = G4NistManager::Instance()->GetMaterial(imat) ;
    G4String matname = mat->GetName() ;
    G4double T   = mat->GetTemperature() ;
    G4double rho = mat->GetDensity() ;
    G4double P   = mat->GetPressure() ;
  
    matdirs.push_back(_materials->mkdir(matname)) ;
    matdirs[imat]->cd() ;
    TParameter<double> *TemperaturePar = new TParameter<double>("Temperature",T) ;
    TemperaturePar->Write() ;
    TParameter<double> *DensityPar     = new TParameter<double>("Density",rho / (g/cm3)) ;
    DensityPar->Write() ;
    TParameter<double> *PressurePar    = new TParameter<double>("Pressure",P / bar) ;
    PressurePar->Write() ;
    // Disect the material.
    size_t nele = mat->GetNumberOfElements() ;
    G4ElementVector *elems       = (G4ElementVector*)mat->GetElementVector() ;
    G4double        *fractionVec = (G4double*)mat->GetFractionVector() ;

    for(size_t iele=0; iele<nele; iele++) {
      G4String elname = (*elems)[iele]->GetName() ;
      G4double frac   = fractionVec[iele] ;
			//G4cout <<iele<<" elem = "<<(*elems)[iele]->GetName()<<" f = "<<fractionVec[iele]<<G4endl;
      char elFrac[100] ;
      sprintf(elFrac,"f_%s",elname.c_str()) ;
      TParameter<double> *_fracPar = new TParameter<double>((G4String)elFrac,frac) ;
      _fracPar->Write() ;
    }
    
    _materials->cd() ;
  }
  
  _fGeom->cd() ;
}
