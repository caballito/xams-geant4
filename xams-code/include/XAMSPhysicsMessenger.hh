#ifndef __XENON1TPHYSICSMESSENGER_H__
#define __XENON1TPHYSICSMESSENGER_H__
//	
//  XAMSPhysicsMessenger class.
//
//  Allow for setting of preferences in the PhysicsList
//
//  A.P. Colijn 27-07-2011
//

#include <G4UImessenger.hh>
#include <globals.hh>

class XAMSPhysicsList;

class G4UIcommand;
class G4UIdirectory;
class G4UIcmdWithoutParameter;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWith3Vector;
class G4UIcmdWith3VectorAndUnit;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADouble;
class G4UIcmdWithABool;
class G4UIcmdWithoutParameter;

class XAMSPhysicsMessenger: public G4UImessenger {
public:
  XAMSPhysicsMessenger(XAMSPhysicsList *pPhysicsList);
  ~XAMSPhysicsMessenger();
  
  void SetNewValue(G4UIcommand *pCommand, G4String hNewValues);
private:
  XAMSPhysicsList*           m_pPhysicsList;
  G4UIdirectory*             m_pDirectory;
  G4UIcmdWithAString*        m_pEMlowEnergyModelCmd;
  G4UIcmdWithAString*        m_pHadronicModelCmd;
  G4UIcmdWithABool*          m_pCerenkovCmd;
	G4UIcmdWithADoubleAndUnit* m_pTrackingCutCmd ;
	G4UIcmdWithADoubleAndUnit* m_pProductionCutCmd ;
};

#endif // __XENON1TPHYSICSMESSENGER_H__

