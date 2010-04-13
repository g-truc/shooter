//////////////////////////////////////////////////////////////////////////////
//	Shoot(r)
//////////////////////////////////////////////////////////////////////////////
//! \author Groove (http://www.g-truc.net) / Code(r) (http://www.codermag.com)
//! \date 12/05/2004
//! \file ctrl_particule.cpp
//! \brief Définition du gestionnaire de générateur de particles
//////////////////////////////////////////////////////////////////////////////

#include "affichage.hpp"
#include "ctrl_particule.hpp"
#include "ctrl_fenetre.hpp"
#include "ctrl_jeu.hpp"
#include "fx_etoile.hpp"
#include "fx_explosion.hpp"
#include "fx_fumee.hpp"
#include "texture_manager.hpp"

CParticleManager* CParticleManager::m_pInstance = 0;

CParticleManager::CParticleManager ()
{
	// On initialise tous les pointeurs sur générateur de particules à 0 pour pouvoir vérifier qu'ils sont utilisés ou non.
	for(int i = 0; i < CTRL_GENERATEUR_PARTICULE_NOMBRE; i++)
		this->m_pGenParticule[i] = 0;

	// Chargement de la texture de particules
	this->m_pTexture = textureManager::instance()->createTexture2D(FICHIER_TEXTURE_PARTICULE);

	// Variable permettrant de déactiver l'affichage des particules
	this->m_bActif = true;
}

CParticleManager::~CParticleManager ()
{
	// Destruction des générateurs de particules.
	for(int i = 0; i < CTRL_GENERATEUR_PARTICULE_NOMBRE; i++)
		if(this->m_pGenParticule[i])
			Kill(i);
	// Déchargement de la texture de particules
	textureManager::instance()->release(m_pTexture);
}

// Creation ou récupération de l'instance du gestionnaire de générateurs de particules.
// Si l'instance n'existe pas (m_pInstance == 0), on la créé.
CParticleManager* CParticleManager::Instance()
{
	if(m_pInstance == 0)
		m_pInstance = new CParticleManager;
	return m_pInstance;
}

// Suppression de l'instance du gestionnaire de générateurs de particules si elle existe.
void CParticleManager::Kill()
{
	if(m_pInstance != 0)
	{
		delete m_pInstance;
		m_pInstance = 0;
	}
}

// Offre un accès aux générateurs de particules. On prend soin de vérifier que l'index indentifie bien une entrée de la table
const CParticleSystem* CParticleManager::GetGenerateurParIndex(unsigned int uiIndex) const
{
	if(uiIndex < CTRL_GENERATEUR_PARTICULE_NOMBRE)
		return m_pGenParticule[uiIndex];
	return 0;
}

// Seul cette fonction permet la création d'une instance d'effet de défilement d'étoile
CEtoile* CParticleManager::CreateEtoile()
{
	if(m_bActif)
	{
		// Recherche d'un index libre dans la table des générateurs de particules.
		int iIndex = GetIndexLibre ();
		// S'il y a un index libre, alors on alloue de la mémoire pour l'effet de particules
		return static_cast<CEtoile*> (iIndex != -1 ? m_pGenParticule[iIndex] = new CEtoile (m_pTexture) : 0);
	}
	return 0;
}

// Seul cette fonction permet la création d'une instance d'effet de fumée
CFumee* CParticleManager::CreateFumee()
{
	if(m_bActif)
	{
		// Recherche d'un index libre dans la table des générateurs de particules.
		int iIndex = GetIndexLibre ();
		// S'il y a un index libre, alors on alloue de la mémoire pour l'effet de particules.
		return static_cast<CFumee*> (iIndex != -1 ? m_pGenParticule[iIndex] = new CFumee (m_pTexture) : 0);
	}
	return 0;
}

// Seul cette fonction permet la création d'une instance d'effet d'explosion.
CExplosion* CParticleManager::CreateExplosion()
{
	if(m_bActif)
	{
		// Recherche d'un index libre dans la table des générateurs de particules.
		int iIndex = GetIndexLibre ();
		// S'il y a un index libre, alors on alloue de la mémoire pour l'effet de particules.
		return static_cast<CExplosion*> (iIndex != -1 ? m_pGenParticule[iIndex] = new CExplosion (m_pTexture) : 0);
	}
	return 0;
}

// Seule cette fonction est autorisée a détruire une instance de générateur de particules 
void CParticleManager::Kill(unsigned int uiIndex)
{
	delete m_pGenParticule[uiIndex];
	m_pGenParticule[uiIndex] = 0;
}

// Recherche d'un index libre dans la table des générateurs de particules
int CParticleManager::GetIndexLibre() const
{
	for(int i = 0; i < CTRL_GENERATEUR_PARTICULE_NOMBRE; i++)
		if(m_pGenParticule[i] == 0)
			return i;
	return -1;
}

// Mise à jour des générateurs de particules : Détermine s'il faut les détrire
void CParticleManager::Update()
{
	for(int i = 0; i < CTRL_GENERATEUR_PARTICULE_NOMBRE; i++)
	{
		if(m_pGenParticule[i])
		{
			m_pGenParticule[i]->Update();

			// 27/04/2004 - Suppression des ennemis
			if(m_pGenParticule[i]->GetSupprime())
			{
				Kill(i);
				continue;
			}
		}
	}
}
