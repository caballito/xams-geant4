#include <G4HCofThisEvent.hh>
#include <G4Step.hh>
#include <G4VProcess.hh>
#include <G4ThreeVector.hh>
#include <G4SDManager.hh>
#include <G4ios.hh>

#include <map>

using std::map ;

#include "XAMSSensitiveDetector.hh"

XAMSSensitiveDetector::XAMSSensitiveDetector(G4String hName): G4VSensitiveDetector(hName) {
	collectionName.insert("HitsCollection") ;
}

XAMSSensitiveDetector::~XAMSSensitiveDetector() {

}

void XAMSSensitiveDetector::Initialize(G4HCofThisEvent* pHitsCollectionOfThisEvent) {
	m_pHitsCollection = new XAMSHitsCollection(SensitiveDetectorName, collectionName[0]) ;

	static G4int iHitsCollectionID = -1 ;

	if(iHitsCollectionID < 0)
		iHitsCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]) ;
	
	pHitsCollectionOfThisEvent->AddHitsCollection(iHitsCollectionID, m_pHitsCollection) ;

	m_hParticleTypes.clear() ;
}

G4bool XAMSSensitiveDetector::ProcessHits(G4Step* pStep, G4TouchableHistory*) {
	G4double dEnergyDeposited = pStep->GetTotalEnergyDeposit() ;
	G4Track* pTrack = pStep->GetTrack() ;

	XAMSHit* pHit = new XAMSHit() ;

	pHit->SetTrackId(pTrack->GetTrackID()) ;
//        G4cout <<" Track weight = "<<pTrack->GetWeight()<<G4endl;

	if( !m_hParticleTypes.count(pTrack->GetTrackID()) )
		m_hParticleTypes[pTrack->GetTrackID()] = pTrack->GetDefinition()->GetParticleName() ;

	pHit->SetParentId(pTrack->GetParentID()) ;
	pHit->SetParticleType(pTrack->GetDefinition()->GetParticleName()) ;

	if(pTrack->GetParentID())
		pHit->SetParentType(m_hParticleTypes[pTrack->GetParentID()]) ;
	else
		pHit->SetParentType(G4String("none")) ;

	if(pTrack->GetCreatorProcess())
		pHit->SetCreatorProcess(pTrack->GetCreatorProcess()->GetProcessName()) ;
	else
		pHit->SetCreatorProcess(G4String("Null")) ;

	pHit->SetDepositingProcess(pStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName()) ;
	pHit->SetPosition(pStep->GetPostStepPoint()->GetPosition()) ;
	pHit->SetEnergyDeposited(dEnergyDeposited) ;
	pHit->SetKineticEnergy(pTrack->GetKineticEnergy()) ;
	pHit->SetTime(pTrack->GetGlobalTime()) ;
	// Include the step length.
	// Already a function in G4: G4Step::GetStepLength()
	//G4ThreeVector dPreStepPos = pStep->GetPreStepPoint()->GetPosition() ;
	//G4ThreeVector dPostStepPos = pStep->GetPostStepPoint()->GetPosition() ;
	//G4ThreeVector dDistance = dPostStepPos - dPreStepPos ;
	// Calculate the magnitude, i.e. the absolute value, of the distance.
	//G4double dLength = dDistance.mag() ;
	//pHit->SetStepLength( dLength ) ;
	pHit->SetStepLength(pStep->GetStepLength()) ;
	// Include angle between previous and new step.
	G4ThreeVector dPreDirection = pStep->GetPreStepPoint()->GetMomentumDirection() ;
	G4ThreeVector dPostDirection = pStep->GetPostStepPoint()->GetMomentumDirection() ;
	// Calculate angle between direction unit vectors.
	G4double dAngle = std::acos(dPreDirection * dPostDirection) ;
	pHit->SetAngle(dAngle) ;

	m_pHitsCollection->insert(pHit) ;

	return true ;
}

void XAMSSensitiveDetector::EndOfEvent(G4HCofThisEvent *) {
//  if (verboseLevel>0) { 
//     G4int NbHits = trackerCollection->entries();
//     G4cout << "\n-------->Hits Collection: in this event they are " << NbHits 
//            << " hits in the tracker chambers: " << G4endl;
//     for (G4int i=0;i<NbHits;i++) (*trackerCollection)[i]->Print();
//    } 
}

