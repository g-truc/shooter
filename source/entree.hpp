//////////////////////////////////////////////////////////////////////////////
//	Shoot(r)
//////////////////////////////////////////////////////////////////////////////
//! \author Groove (http://www.g-truc.net) / Code(r) (http://www.codermag.com)
//! \date 12/05/2004
//! \file entree.h
//! \brief Déclaration d'une entree générique.
//////////////////////////////////////////////////////////////////////////////

#ifndef ENTREE_H
#define ENTREE_H

#include "util.hpp"
#include "window_sdl.hpp"

//! Liste des entrées possibles pendant une partie.
typedef enum
{
	ENTREE_NULL = 0,
	ENTREE_HAUT,
	ENTREE_BAS,
	ENTREE_DROIT,
	ENTREE_GAUCHE,
	ENTREE_TIRE,
} EEntree;

//! Interface d'entrée, définissant les façons de communiquer avec le jeu.
class CEntree
{
public:
	CEntree(glm::vec2 const & Position);

	//! Commande de tire par le joueur
	virtual bool GetTirer () {return this->Tirer;}
	//! Commande de déplacement par le joueur
	virtual glm::vec2 GetPosition () {return this->Position;}

protected:
	bool Tirer;

	glm::vec2 Position;
};

#endif //ENTREE_H
