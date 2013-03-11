#ifndef __XENON1TPRIMARYGENERATORACTION_H__
#define __XENON1TPRIMARYGENERATORACTION_H__

#include <G4VUserPrimaryGeneratorAction.hh>
#include <G4GeneralParticleSource.hh>
#include <G4ParticleGun.hh>
#include <G4ThreeVector.hh>
#include <G4Navigator.hh>
#include <globals.hh>
#include <G4EmCalculator.hh>
#include <G4Material.hh>
#include <TFile.h>
#include <TH1.h>
#include <TParameter.h>

#include "XAMSPrimaryGeneratorMessenger.hh"

#define _use_GPS_ 

enum{
  HIST_WEIGHT
};

class G4Event;

class XAMSPrimaryGeneratorAction: public G4VUserPrimaryGeneratorAction {
public:
  XAMSPrimaryGeneratorAction() ;
  ~XAMSPrimaryGeneratorAction() ;

public:
  const long *GetEventSeeds() { return m_lSeeds ; }
  const G4String &GetParticleTypeOfPrimary() { return m_hParticleTypeOfPrimary ; }
  G4double GetEnergyOfPrimary() { return m_dEnergyOfPrimary ; }
  G4ThreeVector GetPositionOfPrimary() { return m_hPositionOfPrimary ; }

  void     GeneratePrimaries(G4Event *pEvent) ;
  void     FillHistograms() ;
  G4double GetNumberOfAcceptedPrimaries(){ return numberOfAcceptedPrimaries ; } ;
private:
  // Communication with messenger.
  XAMSPrimaryGeneratorMessenger *m_pMessenger;

  long          m_lSeeds[2] ;
  G4String      m_hParticleTypeOfPrimary ;
  G4double      m_dEnergyOfPrimary ;
  G4ThreeVector m_hPositionOfPrimary ;
  G4ThreeVector m_hDirectionOfPrimary ;

  G4double      numberOfAcceptedPrimaries ;

  // Histograms.
  TFile      *_f_prim ;
  TDirectory *_generator ;

  TParameter<double> *nAcceptParameter ;

  TH1F       *_cost ;
  TH1F       *_energy ;

  G4GeneralParticleSource *m_pParticleSource ;

};

#endif // __XENON1TPRIMARYGENERATORACTION_H__

