//////////////////////////////////////////////////////////////////////////////
//	Shoot(r)
//////////////////////////////////////////////////////////////////////////////
//! \author Groove (http://www.g-truc.net) / Code(r) (http://www.codermag.com)
//! \date 12/05/2004
//! \file entite_joueur.h
//! \brief Déclaration d'une entité joueur
//////////////////////////////////////////////////////////////////////////////

#ifndef ENTITE_JOUEUR_H
#define ENTITE_JOUEUR_H

#include "ctrl_entite.hpp"
#include "entite.hpp"
#include "fx_fumee.hpp"
#include "type_arme.hpp"
#include "type_joueur.hpp"

#define BOUCLIER_TAILLE 2

//! Entité joueur
class CEntiteJoueur : public entity
{
	//! Seul cette fonction peut accéder aux données membres de la classe, donc c'est le seul qui puisse créer des instances.
	friend CEntiteJoueur * CEntityManager::CreateJoueur();

private:
	CTypeJoueur * m_pTypeJoueur;
	CTypeArme * m_pTypeArme;
	CFumee * m_pFumee;
	CTimer m_TireTimer;
	glm::vec2 m_Desceleration;
	bool m_bTire;

	CEntiteJoueur ();
	virtual ~CEntiteJoueur ();

public:
	//! \brief Création d'une instance d'entité joueur
	static CEntiteJoueur* Create (CTypeJoueur * pTypeJoueur);

	//! \brief Retourne les dommages provoqués par le joueur
	virtual int GetDommage () const;
	//! \brief Retourne la texture de l'entité
	virtual texture * GetTexture () const;
	//! \brief Retourne le type de l'entité
	virtual type getType() const {return entity::JOUEUR;}
	//! \brief Retourne la taille de l'entité
	virtual float GetTaille () const;
	//! \brief Mise à jour de l'entité
	virtual void Update ();
	//! \brief Ajout des dommages à l'entité
	virtual void AddDommage (int iDommage);

	//! \brief Indique si l'entité tire ou non, suivant la commande du joueur
	void SetTirer(bool bTire);
	//! \brief Action de l'entité, pour le joueur c'est un ordre de tire
	void Action();
	//! \brief Retourne la couleur de l'entité
	glm::vec3 GetCouleur() const;
	//! \brief Retourne la texture du bouclier
	texture * GetTextureBouclier() const;
	//! \brief Indique le type de l'arme
	void SetArme(CTypeArme * pTypeArme);
	//! \brief Ajout de points, pour la destruction, pour la destruction d'entité ennemi
	void AjouterPoint(unsigned int uiPoint);
};

#endif //ENTITE_JOUEUR_H
