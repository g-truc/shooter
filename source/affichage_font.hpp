//////////////////////////////////////////////////////////////////////////////
//	Shoot(r)
//////////////////////////////////////////////////////////////////////////////
//! \author Groove (http://www.g-truc.net) / Code(r) (http://www.codermag.com)
//! \date 20/05/2004
//! \file affichage_font.h
//! \brief Déclaration de la classe CFont pour l'affichage de chaînes de charactères formatés.
//////////////////////////////////////////////////////////////////////////////

#ifndef AFFICHAGE_MENU_H
#define AFFICHAGE_MENU_H

#include <cstdarg>
#include <string>

#include "util.hpp"

//! Identifiants de positions prédéfinies à l'écran.
typedef enum
{
	FONT_POSITION_NULL = 0,
	FONT_POSITION_CENTRE,
	FONT_POSITION_CENTRE_HAUT
} EFontPosition;

//! Affichage de chaînes de caractères.
class CFont
{
private:
	unsigned int Texture;
	unsigned int DisplayList;
	glm::vec3 Couleur;

public:
	//! Le constructeur.
	CFont ();
	//! Le destructeur.
	~CFont ();

	//! Indique la couleur d'affichage des caractères.
	void SetCouleur(glm::vec3 const & Couleur);
	//! Chargement d'une image contenant les caractères à afficher.
	void ChargerTexture (std::string const & Filename);
	//! Affichage d'une chaîne de caractères par rapport à une position (X,Y) sur l'écran.
	void Print (int x, int y, const char *szMsg, ...);
	//! Affichage d'une chaîne de caractères par rapport à une position prédéfini sur l'écran.
	void Print (EFontPosition FontPosition, const char *szMsg, ...);
	//! Affichage d'une chaîne de caractères par rapport à une position prédéfini en X et configurable en Y sur l'écran.
	void Print (EFontPosition FontPosition, int y, const char *szMsg, ...);
};

#endif //AFFICHAGE_MENU_H
