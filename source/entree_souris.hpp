//////////////////////////////////////////////////////////////////////////////
//	Shoot(r)
//////////////////////////////////////////////////////////////////////////////
//! \author Groove (http://www.g-truc.net) / Code(r) (http://www.codermag.com)
//! \date 12/05/2004
//! \file entree_souris.h
//! \brief Déclaration d'une entree souris, pour le jeu.
//////////////////////////////////////////////////////////////////////////////

#ifndef ENTREE_SOURIS_H
#define ENTREE_SOURIS_H

#include "entree.hpp"

//! Défini les façons de communiquer avec le jeu avec la souris
class CEntreeSouris : public CEntree
{
private:

public:
	CEntreeSouris();
	CEntreeSouris(glm::vec2 const & Position);

	//! Récupère les événements venant des boutons de la souris
	void Event(SDL_MouseButtonEvent * Event);
	//! Récupère les événements de déplacement de la souris
	void Event(SDL_MouseMotionEvent * Event);
};

#endif //ENTREE_SOURIS_H
