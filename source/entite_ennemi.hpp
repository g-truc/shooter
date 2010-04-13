//////////////////////////////////////////////////////////////////////////////
//	Shoot(r)
//////////////////////////////////////////////////////////////////////////////
//! \author Groove (http://www.g-truc.net) / Code(r) (http://www.codermag.com)
//! \date 12/05/2004
//! \file entite_ennemi.h
//! \brief Déclaration d'une entité ennemi
//////////////////////////////////////////////////////////////////////////////

#ifndef ENTITE_ENNEMI_H
#define ENTITE_ENNEMI_H

#include "ctrl_entite.hpp"
#include "entite.hpp"
#include "type_arme.hpp"
#include "type_ennemi.hpp"

//! Entité ennemi
class CEntiteEnnemi : public entity
{
	//! Seul cette fonction peut accéder aux données membres de la classe, donc c'est le seul qui puisse créer des instances.
	friend CEntiteEnnemi * CEntityManager::CreateEnnemi ();

private:
	const CTypeEnnemi * m_pTypeEnnemi;
	CTimer m_TireTimer;

	CEntiteEnnemi ();
	virtual ~CEntiteEnnemi ();

public:
	//! Création d'une instance d'entité ennemi
	static CEntiteEnnemi* Create (const CTypeEnnemi* pTypeEnnemi, const glm::vec2& Position);

	//! Retourne les dommages provoqués par l'ennemi
	virtual int GetDommage () const;
	//! Retourne la texture de l'entité
	virtual texture * GetTexture () const;
	//! Retourne le type de l'entité
	virtual type getType() const {return entity::ENNEMI;}
	//! Retourne la taille de l'entité
	virtual float GetTaille () const;

    void Action ();
	unsigned int GetPoint () const;
};

#endif //ENTITE_ENNEMI_H
