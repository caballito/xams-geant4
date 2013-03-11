#ifndef XAMSDetectorMessenger_h
#define XAMSDetectorMessenger_h 1

#include <G4UImessenger.hh>
#include <globals.hh>

class XAMSDetectorConstruction;

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

class XAMSDetectorMessenger: public G4UImessenger
{
public:
	XAMSDetectorMessenger(XAMSDetectorConstruction *pXeDetector);
	~XAMSDetectorMessenger();

	void SetNewValue(G4UIcommand *pUIcommand, G4String hString);
//  G4String GetCurrentValue(G4UIcommand *pUIcommand);
private:
	XAMSDetectorConstruction* m_pXeDetector;

	G4UIdirectory *m_pDetectorDir;

	G4UIcmdWithADouble *m_pTeflonReflectivityCmd;
	G4UIcmdWithABool *m_pLXeScintillationCmd;
	G4UIcmdWithADoubleAndUnit *m_pLXeAbsorbtionLengthCmd;
	G4UIcmdWithADoubleAndUnit *m_pLXeRayScatterLengthCmd;
	
  // APC 25-07-2011
	//
	// In order to run with different materials and wall thicknesses
	//
  // do we want tot check for overlapping objects?
  G4UIcmdWithABool          *m_pCheckOverlapCmd;

	G4UIcmdWithADoubleAndUnit* m_pLiquidLevelCmd ;
	G4UIcmdWithAnInteger*      m_pNbrCryostatLayersCmd ;
	
};

#endif

