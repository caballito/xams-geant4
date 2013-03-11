#ifndef __XENON1TPRIMARYGENERATORMESSENGER_H__
#define __XENON1TPRIMARYGENERATORMESSENGER_H__

#include <G4UImessenger.hh>
#include <globals.hh>

class XAMSPrimaryGeneratorAction;

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

class XAMSPrimaryGeneratorMessenger: public G4UImessenger
{
public:
  XAMSPrimaryGeneratorMessenger(XAMSPrimaryGeneratorAction *pPrimaryGeneratorAction);
  ~XAMSPrimaryGeneratorMessenger();
  
  void SetNewValue(G4UIcommand *pCommand, G4String hNewValues);

private:
  XAMSPrimaryGeneratorAction *m_pPrimaryGeneratorAction;
  G4UIdirectory                 *m_pDirectory;
  G4UIcmdWithABool              *m_pVarianceReductionCmd;
  G4UIcmdWithAnInteger          *m_pVarianceReductionModeCmd;
  G4UIcmdWithADouble            *p_survival_cutCmd;
  G4UIcmdWith3VectorAndUnit     *ft_positionCmd;
  G4UIcmdWithADoubleAndUnit     *ft_lengthCmd;
  G4UIcmdWithADoubleAndUnit     *ft_radiusCmd;

};

#endif 

