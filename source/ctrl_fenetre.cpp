//////////////////////////////////////////////////////////////////////////////
//	Shoot(r)
//////////////////////////////////////////////////////////////////////////////
//! \author Groove (http://www.g-truc.net) / Code(r) (http://www.codermag.com)
//! \date 20/05/2004
//! \file ctrl_fenetre.cpp
//! \brief Déclaration de la classe CFenetre pour la création d'une fenêtre, la gestion des entrées, le menu
//////////////////////////////////////////////////////////////////////////////

#include "affichage.hpp"
#include "ctrl_fenetre.hpp"
#include "ctrl_son.hpp"
#include "ctrl_jeu.hpp"
#include "window_sdl.hpp"
#include "texture_manager.hpp"
#include <cstdlib>
/*
#ifdef _DEBUG
#define FULLSCREEN false
#else 
#define FULLSCREEN true
#endif //_DEBUG
*/
#define FULLSCREEN false

CFenetre::CFenetre ()
{
	// Création d'une fenêtre
	CWindowSDL::Instance ()->Create(FENETRE_NOM, FENETRE_LARGEUR, FENETRE_HAUTEUR, FENETRE_COULEUR, FULLSCREEN);

	// Lecture de la musique du menu
	CSoundManager::Instance ()->Jouer(FICHIER_MUSIQUE_MENU, CANAL_MUSIQUE, true);

	// Initialisation du menu
	MenuInit ();

	m_Page = PAGE_PRINCIPALE;
	m_bEnd = false;

	// La boucle d'affichage
	while (!m_bEnd)
	{
		// On exécute la page sur lequelle on se situe
		switch (m_Page)
		{
		default:
		case PAGE_NULL:
			m_Page = PAGE_QUITTER;
			break;
		case PAGE_JEU_CHARGEMENT:
			if (!PageJeuChargement ())
				m_Page = PAGE_JEU_CHARGEMENT_ERREUR;
			break;
		case PAGE_JEU_CHARGEMENT_ERREUR:
			PageJeuChargementErreur ();
			break;
		case PAGE_JEU_ENCOURS:
			PageJeuEnCours ();
			break;
		case PAGE_PRINCIPALE:
			PagePrincipale ();
			break;
		case PAGE_JEU_COMMENCER:
			PageJeuCommencer ();
			break;
		case PAGE_SCORE:
			PageScore ();
			break;
		case PAGE_QUITTER:
			PageQuitter ();
			break;
		}
	}
}

CFenetre::~CFenetre ()
{
//	CGameManager::Kill ();
	textureManager::destroy();
	CAffichage::Kill ();
	// Si une instance de CGameManager a été créé alors on la supprime
	CSoundManager::Kill ();
	CWindowSDL::Kill ();
	SDL_Quit ();

	printf ("Shoot(r) :)\n");
}

void CFenetre::MenuInit ()
{
	m_uiMenuPrincipal = PAGE_PRINCIPALE_JEU_COMMENCER;
	m_uiMenuCommencer = PAGE_JEU_COMMENCER_NOMBRE_JOUEUR;
	m_uiNombreJoueur = 1;

	m_pTextureMenuFond = textureManager::instance()->createTexture2D(FICHIER_TEXTURE_FOND_MENU);
}

bool CFenetre::PageJeuChargement ()
{
	// Affichage de l'image de fond du menu
	CAffichage *pAffichage = CAffichage::Instance ();
	pAffichage->Debut ();
	pAffichage->MenuImageFond (m_pTextureMenuFond);
	pAffichage->MenuChargement ();
	pAffichage->Fin ();

	// Initialisation des entrées pour le jeu
	m_EntreeSouris = CEntreeSouris(glm::vec2(FENETRE_LARGEUR * 1 / 3, FENETRE_HAUTEUR / 6));
	m_EntreeClavier = CEntreeClavier(glm::vec2(FENETRE_LARGEUR * 2 / 3, FENETRE_HAUTEUR / 6));

	// Création d'une nouvelle instance de Jeu
	// Nous commencons par détruire l'ancienne, s'il y en avait une
	CGameManager::Kill ();
	// On créé une nouvelle instance de CGameManager.
	CGameManager *pJeu = CGameManager::Instance ();

	// Initialisation du jeu
	if (!pJeu->Init (m_uiNombreJoueur, m_szNomJoueur1.c_str (), m_szNomJoueur2.c_str ()))
	{
		pJeu->Kill ();
		return false;
	}

	// Lecture de la musique du jeu.
	CSoundManager::Instance()->Jouer(FICHIER_MUSIQUE_JEU, CANAL_MUSIQUE, true);

	m_Page = PAGE_JEU_ENCOURS;
	return true;
}

void CFenetre::PageJeuChargementErreur()
{
	SDL_Event event;
	bool bFin = false;

	// Affichage de l'image de fond
	CAffichage *pAffichage = CAffichage::Instance ();
	pAffichage->Debut ();
	pAffichage->MenuImageFond (m_pTextureMenuFond);
	pAffichage->MenuChargementErreur ();
	pAffichage->Fin ();

	while(!bFin)
	{
		// Récupération des événements provoqués par les utilisateurs
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
			case SDL_QUIT:
			case SDL_KEYDOWN:
				m_Page = PAGE_PRINCIPALE;
				bFin = true;
				break;
			default:
				break;
			}
		}
	}
}

void CFenetre::PageJeuEnCours ()
{
	SDL_Event event;

	// Récupération des événements provoqués par les joueurs
	while(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
		case SDL_QUIT:
			m_Page = PAGE_PRINCIPALE;
			break;
		case SDL_KEYUP:
		case SDL_KEYDOWN:
			if(event.key.keysym.sym == SDLK_ESCAPE)
			{
				// Lecture de la musique du menu
				CSoundManager::Instance()->Jouer(FICHIER_MUSIQUE_MENU, CANAL_MUSIQUE, true);
				m_Page = PAGE_PRINCIPALE;
			}
			m_EntreeClavier.Event((SDL_KeyboardEvent*) &event);
			break;
		case SDL_MOUSEMOTION:
			m_EntreeSouris.Event((SDL_MouseMotionEvent*) &event);
			break;
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
			m_EntreeSouris.Event((SDL_MouseButtonEvent*) &event);
			break;
		default:
			break;
		}
	}

	CGameManager *pJeu = CGameManager::Instance();

	if(pJeu->IsNiveauFin() && m_EntreeClavier.Start())
	{
		pJeu->ChangerNiveau();
	}

	// Indique si le joueur 1 tire.
	pJeu->Tirer(JOUEUR_1, m_EntreeSouris.GetTirer());
	// Indique si le joueur 2 tire.
	pJeu->Tirer(JOUEUR_2, m_EntreeClavier.GetTirer());

	// Positionne le joueur 1 suivant la position de la souris
	pJeu->PositionnerJoueur(JOUEUR_1, m_EntreeSouris.GetPosition());
	// Positionne le joueur 2 suivant la position obtenu avec le clavier
	pJeu->PositionnerJoueur(JOUEUR_2, m_EntreeClavier.GetPosition());

	// Mise à jour du jeu (niveaux, entités, état du jeu)
	pJeu->Update();
}

void CFenetre::PagePrincipale()
{
	SDL_Event event;
	bool bAction = false;

	while(SDL_PollEvent(&event))
	{
		// Récupération des événements provoqués par les utilisateurs
		switch(event.type)
		{
		case SDL_QUIT:
			m_Page = PAGE_QUITTER;
			break;
		case SDL_KEYDOWN:
			// Déplacement vers le haut dans le menu
			if(event.key.keysym.sym == SDLK_KP8 || event.key.keysym.sym == SDLK_UP)
			{
				unsigned int iMax = CGameManager::IsCreate () != 0 ? PAGE_PRINCIPALE_JEU_REPRENDRE : PAGE_PRINCIPALE_JEU_COMMENCER;
				m_uiMenuPrincipal++;
				if (m_uiMenuPrincipal > iMax)
					m_uiMenuPrincipal = iMax;
			}
			// Déplacement vers la bas dans le menu
			if(event.key.keysym.sym == SDLK_KP2 || event.key.keysym.sym == SDLK_DOWN)
			{
				m_uiMenuPrincipal--;
				if (m_uiMenuPrincipal < PAGE_PRINCIPALE_QUITTER)
					m_uiMenuPrincipal = PAGE_PRINCIPALE_QUITTER;
			}
			// Commande une action, pour le changement de menu par exemple
			if(event.key.keysym.sym == SDLK_SPACE
				|| event.key.keysym.sym == SDLK_RETURN
				|| event.key.keysym.sym == SDLK_KP_ENTER)
			{
				bAction = true;
			}
			if(event.key.keysym.sym == SDLK_ESCAPE)
				m_Page = PAGE_QUITTER;
			break;
		default:
			break;
		}
	}

	// Commence le chargement de page dans le menu
	if(bAction)
	{
		switch(m_uiMenuPrincipal)
		{
		default:
			break;
		case PAGE_PRINCIPALE_QUITTER:
			m_Page = PAGE_QUITTER;
			break;
		case PAGE_PRINCIPALE_SCORES:
			m_Page = PAGE_SCORE;
			break;
		case PAGE_PRINCIPALE_JEU_COMMENCER:
			m_Page = PAGE_JEU_COMMENCER;
			break;
		case PAGE_PRINCIPALE_JEU_REPRENDRE:
			m_Page = PAGE_JEU_ENCOURS;
			break;
		}
	}

	// Affichage de la page principale
	CAffichage * pAffichage = CAffichage::Instance();
	pAffichage->Debut();
	pAffichage->MenuImageFond(m_pTextureMenuFond);
	pAffichage->MenuPrincipal(m_uiMenuPrincipal, CGameManager::IsCreate());
	pAffichage->Fin();
}

void CFenetre::PageJeuCommencer()
{
	SDL_Event event;
	bool bAction = false;

	// On active la gestion des charactère unicode pour le clavier. Ainsi sur tous les claviers (azerty, qwerty, etc...) a = a et q = q
	// C'est plus pratique pour taper le nom des joueurs
	//SDL_EnableUNICODE (1);
	//SDL_EnableKeyRepeat (SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

	// Récupération des événements provoqués par les utilisateurs
	while(SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		default:
			break;
		case SDL_QUIT:
			m_Page = PAGE_PRINCIPALE;
			//SDL_EnableUNICODE (0);
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE)
			{
				m_Page = PAGE_PRINCIPALE;
				//SDL_EnableUNICODE(0);
			}
			// Efface les noms des joueurs
			else if (event.key.keysym.sym == SDLK_BACKSPACE)
			{
				if (m_uiMenuCommencer == PAGE_JEU_COMMENCER_JOUEUR1)
					m_szNomJoueur1.clear ();
				if (m_uiMenuCommencer == PAGE_JEU_COMMENCER_JOUEUR2)
					m_szNomJoueur2.clear ();
			}
			// Déplacement vers le haut dans le menu
			else if (event.key.keysym.sym == SDLK_KP8 || event.key.keysym.sym == SDLK_UP)
			{
				if (m_uiNombreJoueur == PAGE_JEU_COMMENCER_JOUEUR2 || m_uiMenuCommencer != PAGE_JEU_COMMENCER_CHARGEMENT)
				{
					m_uiMenuCommencer++;
					if (m_uiMenuCommencer > PAGE_JEU_COMMENCER_NOMBRE_JOUEUR)
						m_uiMenuCommencer = PAGE_JEU_COMMENCER_NOMBRE_JOUEUR;
				}
				else
					m_uiMenuCommencer = PAGE_JEU_COMMENCER_JOUEUR1;
			}
			// Déplacement vers la bas dans le menu
			else if (event.key.keysym.sym == SDLK_KP2 || event.key.keysym.sym == SDLK_DOWN)
			{
				if (m_uiNombreJoueur == PAGE_JEU_COMMENCER_JOUEUR2 || m_uiMenuCommencer != PAGE_JEU_COMMENCER_JOUEUR1)
				{
					m_uiMenuCommencer--;
					if (m_uiMenuCommencer < PAGE_JEU_COMMENCER_CHARGEMENT)
						m_uiMenuCommencer = PAGE_JEU_COMMENCER_CHARGEMENT;
				}
				else
					m_uiMenuCommencer = PAGE_JEU_COMMENCER_CHARGEMENT;
			}
			// Commande une action, pour le changement de menu par exemple
			else if (event.key.keysym.sym == SDLK_SPACE
				|| event.key.keysym.sym == SDLK_RETURN
				|| event.key.keysym.sym == SDLK_KP_ENTER)
			{
				bAction = true;
			}
			else if (event.key.keysym.unicode > 32 && event.key.keysym.unicode <= 160)
			{
				// Saisi des lettres des noms des joueurs
				if (m_uiMenuCommencer == PAGE_JEU_COMMENCER_JOUEUR1)
					m_szNomJoueur1.push_back (static_cast<char> (event.key.keysym.unicode));
				if (m_uiMenuCommencer == PAGE_JEU_COMMENCER_JOUEUR2 && m_uiNombreJoueur == 2)
					m_szNomJoueur2.push_back (static_cast<char> (event.key.keysym.unicode));
			}
			break;
		}
	}

	if (bAction)
	{
		switch (m_uiMenuCommencer)
		{
		case PAGE_JEU_COMMENCER_CHARGEMENT:
			m_Page = PAGE_JEU_CHARGEMENT;
			//SDL_EnableUNICODE (0);
			break;
		case 4:
			// on bascule entre ou 2 joueurs
			m_uiNombreJoueur = m_uiNombreJoueur == 1 ? 2 : 1;
			break;
		}
	}

	// Affichage de la page commencer jeu
	CAffichage *pAffichage = CAffichage::Instance ();
	pAffichage->Debut ();
	// Affichage de l'image de fond
	pAffichage->MenuImageFond (m_pTextureMenuFond);
	// Afficahge du menu de commencemant d'une partie
	pAffichage->MenuCommencer (m_uiMenuCommencer, m_uiNombreJoueur, m_szNomJoueur1, m_szNomJoueur2);
	pAffichage->Fin ();
}

void CFenetre::PageScore ()
{
	SDL_Event event;
	bool bFin = false;

	// Affichage de la page score
	CAffichage *pAffichage = CAffichage::Instance ();
	pAffichage->Debut ();
	// Affichage de l'image de fond
	pAffichage->MenuImageFond (m_pTextureMenuFond);
	// Affichage du menu des scores
	pAffichage->MenuScore ();
	pAffichage->Fin ();

	// On attend que l'utilisateur appui sur une touche pour retourner au menu principal
	while (!bFin)
	{
		// Récupération des événements provoqués par les utilisateurs
		while (SDL_PollEvent (&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
			case SDL_KEYDOWN:
				m_Page = PAGE_PRINCIPALE;
				bFin = true;
				break;
			default:
				break;
			}
		}
	}
}

void CFenetre::PageQuitter()
{
	SDL_Event event;

	textureManager *pTextureManager = textureManager::instance();
	m_pTextureMenuQuitter = pTextureManager->createTexture2D(FICHIER_TEXTURE_FOND_QUITTER);

	// Affichage de la page quitter
	CAffichage * pAffichage = CAffichage::Instance();
	pAffichage->Debut();
	// Affichage de l'image de fin
	pAffichage->MenuImageFond(m_pTextureMenuQuitter);
	pAffichage->Fin();

	pTextureManager->release(m_pTextureMenuQuitter);

	// Lecture de la musique de fin
	CSoundManager::Instance()->Jouer(FICHIER_MUSIQUE_QUITTER, CANAL_MUSIQUE, true);

	bool bEnd = false;
	// On attend que l'utilisateur appuie sur une touche pour quitter
	while(!bEnd)
	{
		// Récupération des événements provoqués par les utilisateurs
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
			case SDL_KEYDOWN:
				//Quitter (0);
				bEnd = true;
				m_bEnd = true;
				break;
			default:
				break;
			}
		}
	}
}
