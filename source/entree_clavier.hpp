//////////////////////////////////////////////////////////////////////////////
//	Shoot(r)
//////////////////////////////////////////////////////////////////////////////
//! \author Groove (http://www.g-truc.net) / Code(r) (http://www.codermag.com)
//! \date 12/05/2004
//! \file entree_clavier.h
//! \brief Déclaration d'une entree clavier, pour le jeu.
//////////////////////////////////////////////////////////////////////////////

#ifndef ENTREE_CLAVIER_H
#define ENTREE_CLAVIER_H

#include "entree.hpp"

float const CLAVIER_VITESSE_DEPLACEMENT = 4.0f;

//! Défini les façons de communiquer avec le jeu avec le clavier
class CEntreeClavier : public CEntree
{
private:
	CTimer m_Timer;

	bool m_bHaut;
	bool m_bBas;
	bool m_bGauche;
	bool m_bDroit;
	bool m_bStart;

public:
	CEntreeClavier();
	CEntreeClavier(glm::vec2 const & Position);

	//! Commande de déplacement par le joueur
	virtual glm::vec2 GetPosition ();

	//! Récupère les événements du clavier
	void Event (SDL_KeyboardEvent *pEvent);

	//! Touche start
	bool Start ();
};

#endif //ENTREE_CLAVIER_H

