//////////////////////////////////////////////////////////////////////////////
//	Shoot(r)
//////////////////////////////////////////////////////////////////////////////
//! \author Groove (http://www.g-truc.net) / Code(r) (http://www.codermag.com)
//! \date 12/05/2004
//! \file type_bonus_bouclier.cpp
//! \brief Définition d'un type d'entités bonus bouclier
//////////////////////////////////////////////////////////////////////////////

#include "type_bonus_bouclier.hpp"
#include "texture_manager.hpp"

CTypeBonusBouclier::CTypeBonusBouclier()
{
	m_iBouclier = 1;
}

CTypeBonusBouclier::~CTypeBonusBouclier()
{
	textureManager::instance()->release(m_pTexture);
}

// Quand un joueur entre en contact avec un bonus de bouclier alors le bouclier gagne les points du bonus 
void CTypeBonusBouclier::Contact(entity * pEntite)
{
	if(pEntite->getType() == entity::JOUEUR)
		pEntite->AddDommage(-m_iBouclierBonus);
}

void CTypeBonusBouclier::SetBonusBouclier(int iBouclierBonus)
{
	m_iBouclierBonus = iBouclierBonus;
}
