//////////////////////////////////////////////////////////////////////////////
//	Shoot(r)
//////////////////////////////////////////////////////////////////////////////
//! \author Groove (http://www.g-truc.net) / Code(r) (http://www.codermag.com)
//! \date 12/05/2004
//! \file entite_arme.h
//! \brief Déclaration d'une entité arme
//////////////////////////////////////////////////////////////////////////////

#ifndef ENTITE_ARME_H
#define ENTITE_ARME_H

#include "ctrl_entite.hpp"
#include "entite.hpp"
#include "type_arme.hpp"
#include "fx_fumee.hpp"

//! Entité arme (tire des ennemis et des joueurs)
class CEntiteArme : public entity
{
	//! Seul cette fonction peut accéder aux données membres de la classe, donc c'est le seul qui puisse créer des instances.
	friend CEntiteArme* CEntityManager::CreateArme ();

private:
	CTypeArme const * m_pTypeArme;
	entity * m_pProprietaire;
	type m_EntiteType;
	CFumee * m_pFumee;

	CEntiteArme ();
	~CEntiteArme ();

public:
	//! Création d'une instance d'entité arme
	static CEntiteArme * Create (
		CTypeArme const * pTypeArme, 
		entity * pProprietaire, 
		glm::vec2 const & Ancrage);

	//! Retourne les dommages provoqués par l'armes
	virtual int GetDommage () const;
	//! Retourne la texture de l'entité
	virtual texture * GetTexture () const;
	//! Retourne le type de l'entité
	virtual type getType() const {return entity::ARME;}
	//! Retourne la taille de l'entité
	virtual float GetTaille () const;

	//! Retourne du type du propriétaire de l'entité
	type GetTypeProprietaire () const {return m_EntiteType;}
	//! Retourne l'entité propriétaire de l'entité
	entity * GetProprietaire () const {return m_pProprietaire;}
	//! Indique le propriétaire de l'entité
	void SetProprietaire (entity * Entite) {m_pProprietaire = Entite;}
};

#endif //ENTITE_ARME_H


