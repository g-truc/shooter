//////////////////////////////////////////////////////////////////////////////
//	Shoot(r)
//////////////////////////////////////////////////////////////////////////////
//! \author Groove (http://www.g-truc.net) / Code(r) (http://www.codermag.com)
//! \date 12/05/2004
//! \file fx_etoile.cpp
//! \brief Définition d'un générateur de particules réalisant un effet de défilement d'étoiles
//////////////////////////////////////////////////////////////////////////////

#include "affichage.hpp"
#include "ctrl_fenetre.hpp"
#include "ctrl_particule.hpp"
#include "fx_etoile.hpp"

CEtoile::CEtoile(texture * Texture)
{
	this->m_pTexture = Texture;
}

CEtoile::~CEtoile ()
{

}

// Création d'une instance d'effet de défilement d'étoiles
const CEtoile* CEtoile::Create()
{
	// Demande au gestionnaire d'effets de particules de créer une instance d'effet de particules puis récupère une pointeur sur elle
	CEtoile * pGenerateurParticule = CParticleManager::Instance ()->CreateEtoile();
	if(pGenerateurParticule == 0)
		return 0;

	pGenerateurParticule->m_fTailleParticule = 4;
	pGenerateurParticule->m_fVitesseMin = 48;
	pGenerateurParticule->m_fVitesseMax = 96;
	pGenerateurParticule->m_fDureeVie = 100.f;
	pGenerateurParticule->m_Couleur = glm::vec3(1.0f, 1.0f, 1.0f);

	return pGenerateurParticule;	
}

// Initialisation de l'effet de particules
void CEtoile::Init()
{

}

// Mise à jour de l'effet de particules
void CEtoile::Update()
{
	for(int i = 0; i < PARTICULE_NOMBRE; i++)
	{
		if(m_pParticule[i])
		{
			// Suppression des particules quand elle sorte de l'écran
			if(m_pParticule[i]->GetPosition ().y < 0)
			{
				delete m_pParticule[i];
				m_pParticule[i] = 0;
				continue;
			}
		}
	}

	if(m_Timer.GetTime() > 0.1)
	{
		m_Timer.Init();
		int iIndex = GetIndexLibre();
		if(iIndex != -1)
			m_pParticule[iIndex] = new CParticule(
			glm::vec2(glm::compRand1(0, FENETRE_LARGEUR), FENETRE_HAUTEUR), 
			glm::vec2(0, -glm::compRand1(m_fVitesseMin, m_fVitesseMax)), 
			glm::vec2(0, 0), 5, 10);
	}
}

glm::vec3 CEtoile::GetCouleur() const
{
	return glm::vec3(glm::compRand3(0.2f, 1.0f));
}

