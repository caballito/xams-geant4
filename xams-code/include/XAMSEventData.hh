#ifndef __EVENTDATA_H__
#define __EVENTDATA_H__

#include <string>
#include <vector>

using std::string;
using std::vector;

class XAMSEventData
{
public:
	 XAMSEventData();
	~XAMSEventData();

public:
	void Clear();

public:
	int m_iEventId;	// The event ID.
	float m_fTotalEnergyDeposited;	// Total energy deposited in the SD.
	int m_iNbSteps;	// Number of energy depositing steps.
	vector<int> *m_pTrackId;	// ID of the particle.
	vector<int> *m_pParentId;	// ID of the parent particle.
	vector<string> *m_pParticleType;	// Type of particle.
	vector<string> *m_pParentType;	// type of parent particle.
	vector<string> *m_pCreatorProcess;	// Creator process.
	vector<string> *m_pDepositingProcess;	// Energy depositing process.
	vector<float> *m_pX;	// Position of the step.
	vector<float> *m_pY;
	vector<float> *m_pZ;
	vector<float> *m_pEnergyDeposited;	// Energy deposited in the step.
	vector<float> *m_pKineticEnergy;	// Particle's kinetic energy after the step.
	vector<float> *m_pTime;	// Time of the step.
	vector<float> *m_pStepLength ;	// Spatial distance of the step.
	vector<string> *m_pPrimaryParticleType;	// Type of primary particle.
	float m_fPrimaryX;	// Position of the primary particle.
	float m_fPrimaryY;
	float m_fPrimaryZ;	
  float m_fPrimaryE;	// Energy of the primary.
  float m_fPrimaryW;	// Weight of the primary.

};

#endif // __XENON10PEVENTDATA_H__

