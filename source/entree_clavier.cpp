//////////////////////////////////////////////////////////////////////////////
//	Shoot(r)
//////////////////////////////////////////////////////////////////////////////
//! \author Groove (http://www.g-truc.net) / Code(r) (http://www.codermag.com)
//! \date 12/05/2004
//! \file entree_clavier.cpp
//! \brief Définition d'une entree clavier, pour le jeu.
//////////////////////////////////////////////////////////////////////////////

#include "ctrl_fenetre.hpp"
#include "entree_clavier.hpp"

CEntreeClavier::CEntreeClavier() :
	CEntree(Position)
{}

CEntreeClavier::CEntreeClavier
(
	glm::vec2 const & Position
) :
	CEntree(Position)
{
	m_bStart = false;
	m_bHaut = false;
	m_bBas = false;
	m_bGauche = false;
	m_bDroit = false;
	m_Timer.Init ();
}

void CEntreeClavier::Event(SDL_KeyboardEvent * Event)
{
	switch(Event->type)
	{
	default:
		break;
	// Evénement produit quand le joueur relache une touche
	case SDL_KEYUP:
		if(Event->keysym.sym == SDLK_SPACE
			|| Event->keysym.sym == SDLK_KP_ENTER
			|| Event->keysym.sym == SDLK_KP0)
			this->Tirer = false;
		else if(Event->keysym.sym == SDLK_KP8
			|| Event->keysym.sym == SDLK_UP)
			m_bHaut = false;
		else if(Event->keysym.sym == SDLK_KP2
			|| Event->keysym.sym == SDLK_DOWN)
			m_bBas = false;
		else if(Event->keysym.sym == SDLK_KP6
			|| Event->keysym.sym == SDLK_RIGHT)
			m_bDroit = false;
		else if(Event->keysym.sym == SDLK_KP4
			|| Event->keysym.sym == SDLK_LEFT)
			m_bGauche = false;
		else if(Event->keysym.sym == SDLK_RETURN)
			m_bStart = false;
		break;
	// Evénement produit quand le joueur presse une touche
	case SDL_KEYDOWN:
		if(Event->keysym.sym == SDLK_SPACE
			|| Event->keysym.sym == SDLK_KP_ENTER
			|| Event->keysym.sym == SDLK_KP0)
			this->Tirer = true;
		else if(Event->keysym.sym == SDLK_KP8
			|| Event->keysym.sym == SDLK_UP)
			m_bHaut = true;
		else if(Event->keysym.sym == SDLK_KP2
			|| Event->keysym.sym == SDLK_DOWN)
			m_bBas = true;
		else if(Event->keysym.sym == SDLK_KP6
			|| Event->keysym.sym == SDLK_RIGHT)
			m_bDroit = true;
		else if(Event->keysym.sym == SDLK_KP4
			|| Event->keysym.sym == SDLK_LEFT)
			m_bGauche = true;
		else if(Event->keysym.sym == SDLK_RETURN)
			m_bStart = true;
		break;
	}
}

// Détermine la position du joueur sur l'écran en s'assurant qu'il ne sorte pas de l'écran
glm::vec2 CEntreeClavier::GetPosition()
{
	if(m_Timer.GetTime() > 0.01f)
	{
		m_Timer.Init();

		// Déplacement suivant les touches pressés
		if(m_bHaut)
			this->Position.y += CLAVIER_VITESSE_DEPLACEMENT;
		if(m_bBas)
			this->Position.y -= CLAVIER_VITESSE_DEPLACEMENT;
		if(m_bGauche)
			this->Position.x -= CLAVIER_VITESSE_DEPLACEMENT;
		if(m_bDroit)
			this->Position.x += CLAVIER_VITESSE_DEPLACEMENT;

		// Contrôle la position du joueur sur l'écran
		if(this->Position.y > (FENETRE_HAUTEUR - 32))
			this->Position.y = FENETRE_HAUTEUR - 32;
		if(this->Position.y < (0 + 32))
			this->Position.y = 0 + 32;
		if(this->Position.x > (FENETRE_LARGEUR - 32))
			this->Position.x = FENETRE_LARGEUR - 32;
		if(this->Position.x < (0 + 32))
			this->Position.x = 0 + 32;
	}

	return this->Position;
}

bool CEntreeClavier::Start()
{
	return m_bStart;
}
