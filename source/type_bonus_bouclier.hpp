//////////////////////////////////////////////////////////////////////////////
//	Shoot(r)
//////////////////////////////////////////////////////////////////////////////
//! \author Groove (http://www.g-truc.net) / Code(r) (http://www.codermag.com)
//! \date 12/05/2004
//! \file type_bonus_bouclier.h
//! \brief Déclaration d'un type d'entités bonus de boucliers
//////////////////////////////////////////////////////////////////////////////

#ifndef TYPE_BONUS_BOUCLIER_H
#define TYPE_BONUS_BOUCLIER_H

#include "type_bonus.hpp"
#include "entite.hpp"

//! \brief Type d'entités bonus boucliers pour des entités de types CEntiteBonus
//! \see CEntiteBonus
class CTypeBonusBouclier : public CTypeBonus
{
private:
	int m_iBouclierBonus;

public:
	CTypeBonusBouclier ();
	virtual ~CTypeBonusBouclier();

	//! Executer quand il y a contact entre une entité joueur et entité bonus
	//! \param [in] pEntite Entité en contact avec l'entité de type bonus bouclier
	virtual void Contact(entity * pEntite);

	//! Indique le nombre de point de boucliers contenu dans le bonus
	void SetBonusBouclier (int iBouclierBonus);
	//! Retourne le nombre de point contenu dans le bonus
	int GetBonusBouclier () const {return m_iBouclierBonus;}
};

#endif //TYPE_BONUS_BOUCLIER_H

