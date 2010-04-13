//////////////////////////////////////////////////////////////////////////////
//	Shoot(r)
//////////////////////////////////////////////////////////////////////////////
//! \author Groove (http://www.g-truc.net) / Code(r) (http://www.codermag.com)
//! \date 24/05/2004
//! \file affichage.h
//! \brief Déclaration de la classe CAffichage pour le menu, le HUD, les particules.
//////////////////////////////////////////////////////////////////////////////

#ifndef AFFICHAGE_H
#define AFFICHAGE_H

#include "affichage_font.hpp"
#include "texture.hpp"
#include "util_timer.hpp"

//! Affichage du menu, du HUD, des particules et des entités.
class CAffichage
{
private:
	static CAffichage *m_pInstance;

	CFont m_Font;
	int m_iFpsNiveau;
	int m_iFps;
	int m_iFpsCompte;
	CTimer m_FpsTimer;
	CTimer m_JeuTimer;

	bool m_bPointSprite;

	//! Affiche le nombre d'image par seconde (Frame per second).
	void Fps ();

	//! Affichage des entités sous forme de Quadrange, si GL_ARB_point_sprite n'est pas supporté.
	void EntiteQuad () const;
	//! Affichage des entités sous forme de point, si GL_ARB_point_sprite est supporté.
	void EntitePoint () const;
	//! Affichage des particules sous forme de Quadrange, si GL_ARB_point_sprite n'est pas supporté.
	void ParticuleQuad () const;
	//! Affichage des particules sous forme de point, si GL_ARB_point_sprite est supporté.
	void ParticulePoint () const;

	CAffichage ();
	~CAffichage () {}

public:
	//! Creation ou récupération de l'instance de l'affichage.
	static CAffichage* Instance ();
	//! Suppression de l'instance de l'affichage si elle existe.
	static void	Kill ();	

	//! Affichage des entités.
	void Entite () const;
	//! Affichage des particules.
	void Particule () const;

	//! Affichage du HUD en cours de jeu pour une partie à deux joueurs.
	void HUDJeu (unsigned int uiNiveau, unsigned int uiJoueur1, const char* szJoueur1, unsigned int uiJoueur2, const char* szJoueur2);
	//! Affichage du HUD en cours de jeu pour une partie à un joueur.
	void HUDJeu (unsigned int uiNiveau, unsigned int uiJoueur1, const char* szJoueur1);
	//! Affichage du HUD de victoire pour une partie à deux joueurs.
	void HUDVictoire (unsigned int uiJoueur1, const char* szJoueur1, unsigned int uiJoueur2, const char* szJoueur2);
	//! Affichage du HUD de victoire pour une partie à un joueur.
	void HUDVictoire (unsigned int uiJoueur1, const char* szJoueur1);
	//! Affichage du HUD de défaites pour une partie à deux joueurs.
	void HUDDefaite (unsigned int uiJoueur1, const char* szJoueur1, unsigned int uiJoueur2, const char* szJoueur2);
	//! Affichage du HUD de défaites pour une partie à un joueur.
	void HUDDefaite (unsigned int uiJoueur1, const char* szJoueur1);
	//! Affichage du HUD lors de la fin d'un niveau pour une partie à deux joueurs.
	void HUDFinNiveau (unsigned int uiNiveau, unsigned int uiJoueur1, const char* szJoueur1, unsigned int uiJoueur2, const char* szJoueur2);
	//! Affichage du HUD lors de la fin d'un niveau pour une partie à un joueur.
	void HUDFinNiveau (unsigned int uiNiveau, unsigned int uiJoueur1, const char* szJoueur1);

	//! Affichage de la page menu principale.
	void MenuPrincipal (unsigned int uiCurseur, bool bJeuCree);
	//! Affichage de la page de chargement.
	void MenuChargement ();
	//! Affichage de la page de commencement d'une partie.
	void MenuCommencer (unsigned int uiCurseur, unsigned int m_uiNombreJoueur, const std::string & szNom1, const std::string & szNom2);
	//! Affichage de la page d'erreurs au cours du chargement.
	void MenuChargementErreur ();
	//! Affichage de la page de scores.
	void MenuScore ();
	//! Affichage d'un image de fond au menu.
	void MenuImageFond (texture * Texture) const;

	//! Debut de la création d'une frame.
	void Debut () const;
	//! Fin de la création d'une frame et affichage de celle ci.
	void Fin () const;
/*
	//! Chargement d'un image TGA 24 bits et retour d'un identifant d'objet de texture.
	unsigned int ChargerImage24Bits (const char* szFilename);
	//! Chargement d'un image TGA 32 bits et retour d'un identifant d'objet de texture.
	unsigned int ChargerImage32Bits (const char* szFilename);
	//! Dechargment d'une image à partir d'un identifiant d'objet de texture.
	void DechargerImage (unsigned int uiTexture);
*/
};

#endif //AFFICHAGE_H
