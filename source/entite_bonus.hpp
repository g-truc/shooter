//////////////////////////////////////////////////////////////////////////////
//	Shoot(r)
//////////////////////////////////////////////////////////////////////////////
//! \author Groove (http://www.g-truc.net) / Code(r) (http://www.codermag.com)
//! \date 12/05/2004
//! \file entite_bonus.h
//! \brief Déclaration d'une entité bonus.
//////////////////////////////////////////////////////////////////////////////

#ifndef ENTITE_BONUS_H
#define ENTITE_BONUS_H

#include "ctrl_entite.hpp"
#include "entite.hpp"
#include "type_bonus.hpp"

//! Entité bonus contenant soit une arme soit de l'énergie pour le bouclier.
class CEntiteBonus : public entity
{
	//! Seul cette fonction peut accéder aux données membres de la classe, donc c'est le seul qui puisse créer des instances.
	friend CEntiteBonus* CEntityManager::CreateBonus ();

private:
	CTypeBonus *m_pTypeBonus;

	CEntiteBonus ();
	virtual ~CEntiteBonus ();

public:
	//! Création d'une instance d'entité bonus
	static CEntiteBonus* Create(
		CTypeBonus * pTypeBonus, 
		glm::vec2 const & Position);

	//! Retourne les dommages propoqués par le bonus
	virtual int GetDommage() const;
	//! Retourne la texture de l'entité
	virtual texture * GetTexture() const;
	//! Retourne le type de l'entité
	virtual type getType() const {return BONUS;}
	//! Retourne la taille de l'entité
	virtual float GetTaille() const;

	void Contact(entity * pEntite);
};

#endif //ENTITE_BONUS_H

