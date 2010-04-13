//////////////////////////////////////////////////////////////////////////////
//	Shoot(r)
//////////////////////////////////////////////////////////////////////////////
//! \author Groove (http://www.g-truc.net) / Code(r) (http://www.codermag.com)
//! \date 21/05/2004
//! \file fx_fumee.cpp
//! \brief Définition d'un générateur de particules réalisant un effet de fumée
//////////////////////////////////////////////////////////////////////////////

#include "ctrl_particule.hpp"
#include "fx_fumee.hpp"

CFumee::CFumee(texture * pTexture)
{
	this->m_pTexture = pTexture;
}

CFumee::~CFumee()
{

}

CFumee* CFumee::Create
(
	entity * pProprietaire, 
	float fTailleParticule, 
	float fPosition, 
	float fVitesse, 
	float fDureeVie, 
	glm::vec3 const & Couleur
)
{
	// Demande au gestionnaire d'effets de particules de créer une instance d'effet de particules puis récupère une pointeur sur elle
	CFumee * pGenerateurParticule = CParticleManager::Instance()->CreateFumee();
	if(pGenerateurParticule == 0)
		return 0;

	pGenerateurParticule->m_pProprietaire = pProprietaire;
	pGenerateurParticule->m_fTailleParticule = fTailleParticule;
	pGenerateurParticule->m_fPosition = fPosition;
	pGenerateurParticule->m_fVitesse = fVitesse;
	pGenerateurParticule->m_fPrecision = 0.0f;
	pGenerateurParticule->m_fDureeVie = fDureeVie;
	pGenerateurParticule->m_Couleur = Couleur;

	return pGenerateurParticule;
}

// Initialisation de l'effet de particules
void CFumee::Init()
{

}

// Mise à jour de l'effet de particules
void CFumee::Update()
{
	for(int i = 0; i < PARTICULE_NOMBRE; i++)
	{
		if(m_pParticule[i])
		{
			// 07/05/2004 - Suppression des particules
			if(m_pParticule[i]->GetFinVie () || !EstDansEcran (m_pParticule[i]->GetPosition (), m_fTailleParticule))
			{
				delete m_pParticule[i];
				m_pParticule[i] = 0;
				continue;
			}
		}
	}

	// Création d'une particule ...
	// Si l'effet de fumée à un propriétaire
	if(m_pProprietaire)
	{
		// Si le temps entre 2 créations de particules est écoulé
		if(m_Timer.GetTime() > 0.01f)
		{
			m_Timer.Init();
			// S'il existe un index libre
			int iIndex = GetIndexLibre();
			if(iIndex != -1)
				m_pParticule[iIndex] = new CParticule 
					(m_pProprietaire->GetPosition() + glm::vec2(0, m_fPosition), 
					glm::vec2(glm::compRand1(- m_fPrecision * m_fVitesse, m_fPrecision * m_fVitesse), m_fVitesse),  
					glm::vec2(0, 0), 
					m_fDureeVie, 
					m_pProprietaire->GetBouclier() / 200.f + 0.5f);
		}
	}
}

void CFumee::Supprimer()
{
	m_pProprietaire = 0;
	m_bSupprime = true;
}
