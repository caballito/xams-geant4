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

#include "XAMSPrimaryGeneratorMessenger.hh"
#include "XAMSPrimaryGeneratorAction.hh"

XAMSPrimaryGeneratorMessenger::XAMSPrimaryGeneratorMessenger(XAMSPrimaryGeneratorAction *pPrimaryGeneratorAction):
  m_pPrimaryGeneratorAction(pPrimaryGeneratorAction)
{
  G4cout <<"XAMSPrimaryGeneratorMessenger:: Define Primary Generator I/O"<<G4endl;
  // create directory
  m_pDirectory = new G4UIdirectory("/run/forced/");
  m_pDirectory->SetGuidance("Primary generator control commands.");

}

XAMSPrimaryGeneratorMessenger::~XAMSPrimaryGeneratorMessenger()
{
  delete m_pDirectory;
}

void
XAMSPrimaryGeneratorMessenger::SetNewValue(G4UIcommand * command, G4String newValues)
{
}

