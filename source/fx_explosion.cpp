//////////////////////////////////////////////////////////////////////////////
//	Shoot(r)
//////////////////////////////////////////////////////////////////////////////
//! \author Groove (http://www.g-truc.net) / Code(r) (http://www.codermag.com)
//! \date 21/05/2004
//! \file fx_explosion.cpp
//! \brief Déclaration d'un générateur de particules réalisant un effet d'explosion
//////////////////////////////////////////////////////////////////////////////

#include "ctrl_particule.hpp"
#include "fx_explosion.hpp"

CExplosion::CExplosion(texture * pTexture)
{
	m_pTexture = pTexture;
}

CExplosion::~CExplosion()
{

}

const CExplosion* CExplosion::Create
(
	entity * pProprietaire, 
	float fTailleParticule, 
	glm::vec2 const & Position, 
	unsigned int uiForce, 
	float fDureeVie, 
	glm::vec3 const & Couleur
)
{
	// Demande au gestionnaire d'effets de particules de créer une instance d'effet de particules puis récupère une pointeur sur elle
	CExplosion * pGenerateurParticule = CParticleManager::Instance()->CreateExplosion();
	if(pGenerateurParticule == 0)
		return 0;

	pGenerateurParticule->m_fTailleParticule = fTailleParticule;
	pGenerateurParticule->m_Position = Position;
	pGenerateurParticule->m_fDureeVie = fDureeVie;
	pGenerateurParticule->m_uiForce = uiForce;
	pGenerateurParticule->m_Couleur = Couleur;
	pGenerateurParticule->Init();

	return pGenerateurParticule;
}

// Initialisation de l'effet de particules
void CExplosion::Init()
{
	m_DureeVie.Init();
	for(unsigned int i = 0; i < m_uiForce; i++)
	{
		float fRandAngle = glm::compRand1(-3.14159f, 3.14159f);
		float fRandCos = glm::cos(fRandAngle);
		float fRandSin = glm::sin(fRandAngle);
		float fRandForce = glm::compRand1(0.0f, (float)m_uiForce);
		glm::vec2 Vitesse = glm::vec2(fRandCos * fRandForce, fRandSin * fRandForce);

		int iIndex = GetIndexLibre ();
		if(iIndex != -1)
			m_pParticule[iIndex] = new CParticule(m_Position, Vitesse, -Vitesse / 6.0f, m_fDureeVie, 1.0f);
	}
}

// Mise à jour de l'effet de particules
void CExplosion::Update()
{
	if(m_DureeVie.GetTime () > m_fDureeVie)
		m_bSupprime = true;
}

