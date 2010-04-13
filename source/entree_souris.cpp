//////////////////////////////////////////////////////////////////////////////
//	Shoot(r)
//////////////////////////////////////////////////////////////////////////////
//! \author Groove (http://www.g-truc.net) / Code(r) (http://www.codermag.com)
//! \date 12/05/2004
//! \file entree_souris.cpp
//! \brief Définition d'une entree souris, pour le jeu.
//////////////////////////////////////////////////////////////////////////////

#include "ctrl_fenetre.hpp"
#include "entree_souris.hpp"

CEntreeSouris::CEntreeSouris() :
	CEntree(Position)
{}

CEntreeSouris::CEntreeSouris
(
	glm::vec2 const & Position
) :
	CEntree(Position)
{
	SDL_WarpMouse(
		(unsigned short)this->Position.x, 
		(unsigned short)(FENETRE_HAUTEUR - this->Position.y));
}

// Evénement provoqué quand un bouton de souris est pressé
void CEntreeSouris::Event(SDL_MouseButtonEvent * pEvent)
{
	if (pEvent->type == SDL_MOUSEBUTTONDOWN && pEvent->button == SDL_BUTTON_LEFT)
		this->Tirer = true;
	else
		this->Tirer = false;
}

// Evénement provoqué quand la souris est déplacé
void CEntreeSouris::Event(SDL_MouseMotionEvent * pEvent)
{
	int iLargeur = pEvent->x;
	int iHauteur = pEvent->y;

	// Contrôle la position de la souris et déplace le curseur si nécessaire.
	if(iHauteur > (FENETRE_HAUTEUR - 32))
	{
		iHauteur = FENETRE_HAUTEUR - 32;
		SDL_WarpMouse(iLargeur, iHauteur);
	}	
	if(iHauteur < (0 + 32))
	{
		iHauteur = 0 + 32;
		SDL_WarpMouse(iLargeur, iHauteur);
	}
	if (iLargeur > (FENETRE_LARGEUR - 32))
	{
		iLargeur = FENETRE_LARGEUR - 32;
		SDL_WarpMouse(iLargeur, iHauteur);
	}
	if(iLargeur < (0 + 32))
	{
		iLargeur = 0 + 32;
		SDL_WarpMouse(iLargeur, iHauteur);
	}
	
	// Indique la position du joueur
	this->Position.x = static_cast<float>(iLargeur);
	this->Position.y = FENETRE_HAUTEUR - static_cast<float>(iHauteur);
}
