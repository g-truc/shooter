//////////////////////////////////////////////////////////////////////////////
//	Shoot(r)
//////////////////////////////////////////////////////////////////////////////
//! \author Groove (http://www.g-truc.net) / Code(r) (http://www.codermag.com)
//! \date 12/05/2004
//! \file type_bonus_arme.cpp
//! \brief Définition d'un type d'entités bonus d'armes
//////////////////////////////////////////////////////////////////////////////

#include "type_bonus_arme.hpp"
#include "texture_manager.hpp"
#include "entite_joueur.hpp"

CTypeBonusArme::CTypeBonusArme() :
	m_pTypeArme(0)
{}

CTypeBonusArme::~CTypeBonusArme()
{
	textureManager::instance()->release(m_pTexture);
	if(this->m_pTypeArme)
	{
		delete this->m_pTypeArme;
		this->m_pTypeArme = 0;
	}
}

// Quand un joueur entre en contact avec un bonus d'armes alors son arme est remplacé par celle du bonus
void CTypeBonusArme::Contact(entity * pEntite)
{
	if(pEntite->getType() == entity::JOUEUR)
		static_cast<CEntiteJoueur*>(pEntite)->SetArme(m_pTypeArme);
}

void CTypeBonusArme::SetArme(CTypeArme * TypeArme)
{
	this->m_pTypeArme = TypeArme;
}
