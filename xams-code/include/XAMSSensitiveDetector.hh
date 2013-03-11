#ifndef __SENSITIVEDETECTOR_H__
#define __SENSITIVEDETECTOR_H__

#include <G4VSensitiveDetector.hh>

#include "XAMSHit.hh"

class G4Step;
class G4HCofThisEvent;

class XAMSSensitiveDetector: public G4VSensitiveDetector
{
public:
	XAMSSensitiveDetector(G4String hName);
	~XAMSSensitiveDetector();

	void Initialize(G4HCofThisEvent *pHitsCollectionOfThisEvent);
	G4bool ProcessHits(G4Step *pStep, G4TouchableHistory *pHistory);
	void EndOfEvent(G4HCofThisEvent *pHitsCollectionOfThisEvent);

private:
	XAMSHitsCollection* m_pHitsCollection;

	std::map<int,G4String> m_hParticleTypes;
};

#endif // __SENSITIVEDETECTOR_H__

