//////////////////////////////////////////////////////////////////////////////
//	Shoot(r)
//////////////////////////////////////////////////////////////////////////////
//! \author Groove (http://www.g-truc.net) / Code(r) (http://www.codermag.com)
//! \date 12/05/2004
//! \file type_joueur.cpp
//! \brief Définition d'un type d'entités joueurs
//////////////////////////////////////////////////////////////////////////////

#include "type_joueur.hpp"
#include "texture_manager.hpp"

CTypeJoueur::CTypeJoueur()
{
	m_uiScore = 0;
	m_fTaille = 16;
	m_uiNiveau = 0;
}

CTypeJoueur::~CTypeJoueur()
{
	textureManager::instance()->release(m_pTexture);
	textureManager::instance()->release(m_pTextureBouclier);
}

