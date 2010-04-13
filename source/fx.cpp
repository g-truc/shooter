//////////////////////////////////////////////////////////////////////////////
//	Shoot(r)
//////////////////////////////////////////////////////////////////////////////
//! \author Groove (http://www.g-truc.net) / Code(r) (http://www.codermag.com)
//! \date 12/05/2004
//! \file fx.cpp
//! \brief Définition d'un générateur de particules génériques
//////////////////////////////////////////////////////////////////////////////

#include "ctrl_fenetre.hpp"
#include "fx.hpp"
#include <cstdio>

CParticleSystem::CParticleSystem ()
{
	m_bSupprime = false;

	// Initialise tous les pointeurs sur particules à 0 pour pouvoir vérifier qu'ils sont utilisés ou non.
	for (int i = 0; i < PARTICULE_NOMBRE; i++)
		m_pParticule[i] = 0;

	m_Timer.Init ();
}

CParticleSystem::~CParticleSystem ()
{
	// Destruction des particules.
	for (int i = 0; i < PARTICULE_NOMBRE; i++)
	{
		if (m_pParticule[i])
		{
			delete m_pParticule[i];
			m_pParticule[i] = 0;
		}
	}
}

// Offre un accès aux particules. On prend soin de vérifier que l'index indentifie bien une entrée de la table
const CParticule* const CParticleSystem::GetParticuleParIndex (unsigned int uiIndex) const
{
	if (uiIndex < PARTICULE_NOMBRE)
		return m_pParticule[uiIndex];
	return 0;
}

// Recherche d'un index libre dans la table des particules
int CParticleSystem::GetIndexLibre () const
{
	for (int i = 0; i < PARTICULE_NOMBRE; i++)
		if (!m_pParticule[i])
			return i;
	return -1;
}

// Détermine si un objet rond centrée sur Position est dans l'écran
bool CParticleSystem::EstDansEcran (const glm::vec2& Position, float fRayon) const
{
	if (Position.x < (0 - fRayon))
		return false;
	if (Position.x > (FENETRE_LARGEUR + fRayon))
		return false;
	if (Position.y < (0 - fRayon))
		return false;
	if (Position.y > (FENETRE_HAUTEUR + fRayon))
		return false;
	return true;
}
