//////////////////////////////////////////////////////////////////////////////
//	Shoot(r)
//////////////////////////////////////////////////////////////////////////////
//! \author Groove (http://www.g-truc.net) / Code(r) (http://www.codermag.com)
//! \date 12/05/2004
//! \file type_bonus_arme.h
//! \brief Déclaration d'un type d'entités bonus d'armes
//////////////////////////////////////////////////////////////////////////////

#ifndef TYPE_BONUS_ARME_H
#define TYPE_BONUS_ARME_H

#include "type_arme.hpp"
#include "type_bonus.hpp"
#include "entite.hpp"

//! \brief Type d'entités bonus armes pour des entités de types CEntiteBonus
//! \see CEntiteBonus
class CTypeBonusArme : public CTypeBonus
{
private:
	CTypeArme *m_pTypeArme;

public:
	CTypeBonusArme ();
	virtual ~CTypeBonusArme ();

	//! Executer quand il y a contact entre une entité joueur et entité bonus
	virtual void Contact (entity * pEntite);

	//! Indique le type d'armes contenu dans le bonus
	void SetArme (CTypeArme *pTypeArme);
	//! Retourne le type d'armes contenu dans le bonus
	CTypeArme* GetArme () const {return m_pTypeArme;}
};

#endif //TYPE_BONUS_ARME_H
