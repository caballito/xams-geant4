#include <globals.hh>
#include <G4RunManagerKernel.hh>
#include <G4Event.hh>
#include <Randomize.hh>

#include <G4GeneralParticleSource.hh>
#include <G4Material.hh>
#include <G4ThreeVector.hh>
#include <G4TransportationManager.hh>
#include <G4Navigator.hh>
#include <G4VPhysicalVolume.hh>
#include <G4LogicalVolume.hh>
#include <G4PrimaryVertex.hh>
#include <G4ParticleDefinition.hh>
// testing
#include <G4Point3D.hh>
#include <G4Vector3D.hh>
#include <G4SingleParticleSource.hh>

#include "XAMSPrimaryGeneratorAction.hh"
#include "XAMSPrimaryGeneratorMessenger.hh"


XAMSPrimaryGeneratorAction::XAMSPrimaryGeneratorAction() {
  m_pMessenger = new XAMSPrimaryGeneratorMessenger(this) ;
  
  G4cout << 
		"XAMSPrimaryGeneratorAction::XAMSPrimaryGeneratorAction() Use G4GeneralParticleSource "
		<< G4endl ;
  m_pParticleSource = new G4GeneralParticleSource() ;
    
  m_lSeeds[0] = -1 ;
  m_lSeeds[1] = -1 ;

  // Number of rejected primaries (double variables, since it counts the event weights).
  numberOfAcceptedPrimaries = 0. ;
  
  // Histograms.
  _f_prim = new TFile("generator.root","RECREATE") ;
  
  _generator = _f_prim->mkdir("generator") ;
  _generator->cd() ;
  _cost   = new TH1F("_cost","_cost",100,-1,1) ;
  _energy = new TH1F("_energy","_energy",1000,0,5) ;
}

XAMSPrimaryGeneratorAction::~XAMSPrimaryGeneratorAction() {
  _f_prim->cd() ;
  _generator->cd() ;
  
  // Write the source settings to file.
  TNamed *TypePar = new TNamed("SourceType", m_pParticleSource->GetParticleDefinition()->GetParticleName()) ;
  TypePar->Write() ;
  TParameter<G4double> *IntensityPar = new TParameter<G4double>("SourceIntensity", m_pParticleSource->GetCurrentSourceIntensity()) ;
  IntensityPar->Write() ;
  
  nAcceptParameter = new TParameter<double>("naccept",GetNumberOfAcceptedPrimaries()) ;
  nAcceptParameter->Write() ;
  
  _f_prim->cd() ;
  
  _f_prim->Write() ;
  _f_prim->Close() ;
  
  delete m_pParticleSource ;
}

void XAMSPrimaryGeneratorAction::GeneratePrimaries(G4Event *pEvent) {
  // Generate a single particle.
  m_pParticleSource->GeneratePrimaryVertex(pEvent) ;
  
  // particle name of primary
  m_hParticleTypeOfPrimary = m_pParticleSource->GetParticleDefinition()->GetParticleName() ;
  // kinetic energy of primary
  m_dEnergyOfPrimary       = m_pParticleSource->GetParticleEnergy() ;
  // position of primary
  m_hPositionOfPrimary     = m_pParticleSource->GetParticlePosition() ;
  // direction of primary
  m_hDirectionOfPrimary    = m_pParticleSource->GetParticleMomentumDirection() ;
    
  FillHistograms() ;
  
  numberOfAcceptedPrimaries += 1.0 ;
}

void XAMSPrimaryGeneratorAction::FillHistograms() {
  _energy->Fill(m_dEnergyOfPrimary) ;
  _cost->Fill(m_hDirectionOfPrimary.z()) ;
}

