//////////////////////////////////////////////////////////////////////////////
//	Shoot(r)
//////////////////////////////////////////////////////////////////////////////
//! \author Groove (http://www.g-truc.net) / Code(r) (http://www.codermag.com)
//! \date 12/05/2004
//! \file type_bonus.h
//! \brief Déclaration d'un type d'entités bonus
//////////////////////////////////////////////////////////////////////////////

#ifndef TYPE_BONUS_H
#define TYPE_BONUS_H

#include "type.hpp"
#include "entite.hpp"

//! Type d'entités bonus
class CTypeBonus : public CType
{
protected:

public:
	virtual ~CTypeBonus () {}

	//! Executer quand il y a contact entre une entité joueur et entité bonus
	virtual void Contact(entity * pEntite) = 0;
};

#endif //TYPE_BONUS_H
