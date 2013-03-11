#ifndef __XENON1TPHYSICSLIST_H__
#define __XENON1TPHYSICSLIST_H__

#include <G4VUserPhysicsList.hh>
#include <globals.hh>

// Leave this if you want to be able to run the
// old lowE physics list. Needs to be undefined 
// when the old implementation is really removed!
// (probably from geant4.5.x onwards)
//
// A.P. Colijn 26-07-2011

// add line below for backward compatibility. GEANT_VERSION < 9.5.x
//#define __KEEP_OLD_LOWE_EM__
//
#include "XAMSPhysicsMessenger.hh"
#include "G4DecayPhysics.hh"

class G4VPhysicsConstructor ;

class XAMSPhysicsList: public G4VUserPhysicsList {
public:
  XAMSPhysicsList() ;
  ~XAMSPhysicsList() ;

public:
  void SetCuts() ;
	
  // Functions for the messenger interface.
  void SetEMlowEnergyModel(G4String theModel)   { m_hEMlowEnergyModel = theModel ; }
  void SetHadronicModel(G4String theModel)      { m_hHadronicModel = theModel ; }
  void SetCerenkov(G4bool useCerenkov)          { m_bCerenkov = useCerenkov ; }
  void SetTrackingCut(G4double trackingCut)     { m_pTrackingCut = trackingCut ; }
  void SetProductionCut(G4double productionCut) { m_pProductionCut = productionCut ; }

  void MakePhysicsPlots() ;
  void WriteParameter(G4String parName) ;
	
protected:
  void ConstructParticle() ;
  void ConstructProcess() ;

  void ConstructOp() ;

  void AddTransportation() ;

  void SetBuilderList1(G4bool flagHP) ;
  
private:
  G4double cutForGamma ;
  G4double cutForElectron ;
  G4double cutForPositron ;
  G4double cutForProton ;
  G4double cutForAlpha ;
  G4double cutForGenericIon ;

  void ConstructMyBosons() ;
  void ConstructMyLeptons() ;
  void ConstructMyHadrons() ;
  void ConstructMyShortLiveds() ;

private:
  G4int VerboseLevel ;
  G4int OpVerbLevel ;
	
  G4VPhysicsConstructor*               emPhysicsList ;
  G4VPhysicsConstructor*               opPhysicsList ;
  std::vector<G4VPhysicsConstructor*>  hadronPhys ;

  //	G4String                 emName;
	
  XAMSPhysicsMessenger* m_pMessenger ;
  G4String              m_hEMlowEnergyModel ;
  G4String              m_hHadronicModel ;
  G4bool                m_bCerenkov ;
	G4double              m_pTrackingCut ;
	G4double              m_pProductionCut ;
  G4DecayPhysics*       particleList ;
} ;

#endif // __XENON1TPHYSICSLIST_H__

