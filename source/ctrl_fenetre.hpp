//////////////////////////////////////////////////////////////////////////////
//	Shoot(r)
//////////////////////////////////////////////////////////////////////////////
//! \author Groove (http://www.g-truc.net) / Code(r) (http://www.codermag.com)
//! \date 25/05/2004
//! \file ctrl_fenetre.h
//! \brief Déclaration de la classe CFenetre pour la création d'une fenêtre, la gestion des entrées, le menu
//////////////////////////////////////////////////////////////////////////////

#ifndef CTRL_FENETRE_H
#define CTRL_FENETRE_H

#include "entree_clavier.hpp"
#include "entree_souris.hpp"
#include "texture.hpp"
#include "window_sdl.hpp"

#ifdef WIN32
#include <windows.h>
#endif //WIN32
#include <GL/gl.h>
#include "glext.h"

static char const * FENETRE_NOM = "Shoot(r)";
int const FENETRE_LARGEUR	= 800;
int const FENETRE_HAUTEUR	= 600;
int const FENETRE_COULEUR	= 24;

static char const * FICHIER_MUSIQUE_QUITTER = "musique/quitter.ogg";
static char const * FICHIER_MUSIQUE_MENU = "musique/menu.ogg";
static char const * FICHIER_MUSIQUE_JEU = "musique/jeu.ogg";

static char const * FICHIER_TEXTURE_FOND_MENU = "image/menu-fond.tga";
static char const * FICHIER_TEXTURE_FOND_QUITTER = "image/menu-quitter.tga";

//! Identifiants des différentes pages du jeu
enum EPage
{
	PAGE_NULL = 0,
	PAGE_JEU_CHARGEMENT,
	PAGE_JEU_CHARGEMENT_ERREUR,
	PAGE_JEU_COMMENCER,
	PAGE_JEU_ENCOURS,
	PAGE_PRINCIPALE,
	PAGE_SCORE,
	PAGE_QUITTER
};

//! Identifiants des entrées du menu de création d'une partie
enum EPageJeuCommencer
{
	PAGE_JEU_COMMENCER_NULL = 0,
	PAGE_JEU_COMMENCER_CHARGEMENT,
	PAGE_JEU_COMMENCER_JOUEUR2,
	PAGE_JEU_COMMENCER_JOUEUR1,
	PAGE_JEU_COMMENCER_NOMBRE_JOUEUR
};

//! Identifiants des entrées du menu principale
enum EPagePrincipale
{
	PAGE_PRINCIPALE_NULL = 0,
	PAGE_PRINCIPALE_QUITTER,
	PAGE_PRINCIPALE_SCORES,
	PAGE_PRINCIPALE_JEU_COMMENCER,
	PAGE_PRINCIPALE_JEU_REPRENDRE
};

//! Gestionnaire des pages du jeu et controleur principale
class CFenetre
{
private:
	CEntreeClavier m_EntreeClavier;
	CEntreeSouris m_EntreeSouris;

	std::string m_szNomJoueur1;
	std::string m_szNomJoueur2;
	unsigned int m_uiNombreJoueur;

	unsigned short m_unPositionX;
	unsigned short m_unPositionY;

	unsigned int m_uiMenuPrincipal;
	unsigned int m_uiMenuCommencer;
//	unsigned int m_uiImageMenuFond;
	texture * m_pTextureMenuFond;
	texture * m_pTextureMenuQuitter;

	EPage m_Page;
	bool m_bEnd;

	//! Page de chargement du jeu. Lecteur du fichier XML pour les niveaux et chargement des ressources : sons, images
	bool PageJeuChargement();
	//! Page affichage de messages d'erreurs, lors du chargement.
	void PageJeuChargementErreur();
	//! Page l'exercution du jeu
	void PageJeuEnCours();
	//! Page de commencement du jeu, pour indiquer le nombre de joueurs et leurs noms
	void PageJeuCommencer();
	//! Page principale, que l'on atteind au démarrage du jeu.
	void PagePrincipale();
	//! Page affichant les meilleurs scores.
	void PageScore();
	//! Page de fin, pour l'affichage des crédits.
	void PageQuitter();

	//! Initialisation du menu
	void MenuInit();

public:
	//! Constructeur
	CFenetre();
	//! Destructeur
	~CFenetre();
};

#endif //CTRL_FENETRE_H

