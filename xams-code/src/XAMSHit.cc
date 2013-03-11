#include <G4UnitsTable.hh>
#include <G4VVisManager.hh>
#include <G4Circle.hh>
#include <G4Colour.hh>
#include <G4VisAttributes.hh>

#include "XAMSHit.hh"

G4Allocator<XAMSHit> XAMSHitAllocator;

XAMSHit::XAMSHit() {}

XAMSHit::~XAMSHit()
{
	if(m_pParticleType) delete m_pParticleType;
	if(m_pParentType) delete m_pParentType;
	if(m_pCreatorProcess) delete m_pCreatorProcess;
	if(m_pDepositingProcess) delete m_pDepositingProcess;
}

XAMSHit::XAMSHit(const XAMSHit &hXAMSHit):G4VHit()
{
	m_iTrackId = hXAMSHit.m_iTrackId;
	m_iParentId = hXAMSHit.m_iParentId;
	m_pParticleType = hXAMSHit.m_pParticleType;
	m_pParentType = hXAMSHit.m_pParentType ;
	m_pCreatorProcess = hXAMSHit.m_pCreatorProcess ;
	m_pDepositingProcess = hXAMSHit.m_pDepositingProcess ;
	m_hPosition = hXAMSHit.m_hPosition;
	m_dEnergyDeposited = hXAMSHit.m_dEnergyDeposited;
	m_dKineticEnergy = hXAMSHit.m_dKineticEnergy ;
	m_dTime = hXAMSHit.m_dTime;
	// Include step length.
	m_dStepLength = hXAMSHit.m_dStepLength;
}

const XAMSHit &
XAMSHit::operator=(const XAMSHit &hXAMSHit)
{
	m_iTrackId = hXAMSHit.m_iTrackId;
	m_iParentId = hXAMSHit.m_iParentId;
	m_pParticleType = hXAMSHit.m_pParticleType;
	m_pParentType = hXAMSHit.m_pParentType ;
	m_pCreatorProcess = hXAMSHit.m_pCreatorProcess ;
	m_pDepositingProcess = hXAMSHit.m_pDepositingProcess ;
	m_hPosition = hXAMSHit.m_hPosition;
	m_dEnergyDeposited = hXAMSHit.m_dEnergyDeposited;
	m_dKineticEnergy = hXAMSHit.m_dKineticEnergy ;
	m_dTime = hXAMSHit.m_dTime;
	// Include step length.
	m_dStepLength = hXAMSHit.m_dStepLength ;
	
	return *this;
}

G4int
XAMSHit::operator==(const XAMSHit &hXAMSHit) const
{
	return ((this == &hXAMSHit) ? (1) : (0));
}

void XAMSHit::Draw()
{
	G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
	
	if(pVVisManager)
	{
		G4Circle hCircle(m_hPosition);
		G4Colour hColour(1.000, 0.973, 0.184);
		G4VisAttributes hVisAttributes(hColour);
		
		hCircle.SetScreenSize(0.1);
		hCircle.SetFillStyle(G4Circle::filled);
		hCircle.SetVisAttributes(hVisAttributes);
		pVVisManager->Draw(hCircle);
	}
}

void XAMSHit::Print()
{
	G4cout << "-------------------- Crystal hit --------------------" 
		<< "Id: " << m_iTrackId
		<< " Particle: " << *m_pParticleType
		<< " ParentId: " << m_iParentId
		<< " ParentType: " << *m_pParentType << G4endl
		<< "CreatorProcess: " << *m_pCreatorProcess
		<< " DepositingProcess: " << *m_pDepositingProcess << G4endl
		<< "Position: " << m_hPosition.x()/mm
		<< " " << m_hPosition.y()/mm
		<< " " << m_hPosition.z()/mm
		<< " mm" << G4endl
		<< "EnergyDeposited: " << m_dEnergyDeposited/keV << " keV"
		<< " KineticEnergyLeft: " << m_dKineticEnergy/keV << " keV"
		<< " Time: " << m_dTime/s << " s" << G4endl;
}

