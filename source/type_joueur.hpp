//////////////////////////////////////////////////////////////////////////////
//	Shoot(r)
//////////////////////////////////////////////////////////////////////////////
//! \author Groove (http://www.g-truc.net) / Code(r) (http://www.codermag.com)
//! \date 12/05/2004
//! \file type_joueur.h
//! \brief Déclaration d'un type d'entités joueurs
//////////////////////////////////////////////////////////////////////////////

#ifndef TYPE_JOUEUR_H
#define TYPE_JOUEUR_H

#include "type.hpp"
#include "util.hpp"

//! \brief Type d'entités joueurs pour des entités de types CEntiteJoueur
//! \see CEntiteJoueur
class CTypeJoueur : public CType
{
private:
	std::string m_szNom;
	glm::vec3 m_Couleur;
	texture * m_pTextureBouclier;
	unsigned int m_uiScore;
	int m_iDommage;
	int m_iNumero;
	unsigned int m_uiNiveau;
	glm::vec2 m_PositionSpawn;

public:
	CTypeJoueur ();
	virtual ~CTypeJoueur ();

	//! Retourne la couleur d'un type de joueur
	glm::vec3 GetCouleur () const {return m_Couleur;}
	//! Retourne la texture de bouclier
	texture * GetTextureBouclier () const {return m_pTextureBouclier;}
	//! Retourne les dommages provoqués par une collision avec un type de joueur
	int GetDommage () const {return m_iDommage;}
	
	//! Indique la couleur d'un type de joueur
	void SetCouleur (const glm::vec3& Couleur) {m_Couleur = Couleur;}
	//! Indique la texture du bouclier
	void SetTextureBouclier (texture * pTextureBouclier) {m_pTextureBouclier = pTextureBouclier;}
	//! Indique les dommages provoqués par une collision avec un type de joueur
	void SetDommage (int iDommage) {m_iDommage = iDommage;}
	//! Indique le numero du type de joueur
	void SetNumero (int iNumero) {m_iNumero = iNumero;}

	//! Ajout de point au type de joueurs
	void AjouterPoint (unsigned int uiPoint) {m_uiScore += uiPoint;}
	//! Récupère le score du type de joueurs
	unsigned int GetScore () const {return m_uiScore;}
	//! Récupère le numero du type de joueurs
	int GetNumero () const {return m_iNumero;}

	//! Indique le nom du type de joueurs
	void SetNom (const char* szNom) {m_szNom = std::string (szNom);}
	//! Retourne le nom du type de joueurs
	const char* GetNom () const {return m_szNom.c_str ();}

	//! Indique le numéro du niveau atteint par le type de joueurs
	void SetNiveau (unsigned int uiNiveau) {m_uiNiveau = uiNiveau;}
	//! Retourne le numéro du niveau atteint par le type de joueurs
	unsigned int GetNiveau () const {return m_uiNiveau;}

	//! Indique la position d'apparition du type de joueur sur l'écran de jeu
	void SetPositionSpawn (const glm::vec2& Position) {m_PositionSpawn = Position;}
	//! retourne la position d'apparition du type de joueur sur l'écran de jeu
	glm::vec2 GetPositionSpawn () const {return m_PositionSpawn;}
};

#endif //TYPE_JOUEUR_H


