//////////////////////////////////////////////////////////////////////////////
//	Shoot(r)
//////////////////////////////////////////////////////////////////////////////
//! \author Groove (http://www.g-truc.net) / Code(r) (http://www.codermag.com)
//! \date 25/05/2004
//! \file ctrl_jeu.h
//! \brief Déclaration du coeur du projet, CGameManager englobe les niveaux, indique quand créer les entités, détermine le début et la fin d'une partie.
//////////////////////////////////////////////////////////////////////////////

#ifndef CTRL_JEU_H
#define CTRL_JEU_H

#include "xml_niveau.hpp"
#include "entite_joueur.hpp"
#include "type_joueur.hpp"

#define TEMPS_ENTRE_NIVEAU 5.0f
#define TYPE_JOUEUR_NOMBRE 2

#define FICHIER_SON_EXPLOSION "joueur/explosion.wav"
#define FIFHIER_TEXTURE_VAISSEAU "joueur/vaisseau.tga"
#define FIFHIER_TEXTURE_BOUCLIER "joueur/bouclier.tga"

//! Identifiants des états du jeu
typedef enum
{
	ETAT_NULL = 0,
	ETAT_JEU_JEU,
	ETAT_JEU_VICTOIRE,
	ETAT_JEU_DEFAITE,
	ETAT_JEU_FIN_NIVEAU,
} EEtatJeu;

//! Identifiants des joueurs
typedef enum
{
	JOUEUR_NULL = 0,
	JOUEUR_1,
	JOUEUR_2
} EJoueur;

//! Gestionnaires du jeu
class CGameManager
{
private:
	static CGameManager *m_pGameManager;

	CTypeJoueur *m_TypeJoueur;
	unsigned int m_uiTypeJoueur;

	CEntiteJoueur *m_pJoueur1;
	CEntiteJoueur *m_pJoueur2;

	CXMLNiveau m_XMLNiveaux;
	unsigned int m_uiNiveau;
	CTimer m_NiveauTemps;
	EEtatJeu m_Etat;
	bool m_bNiveauChange;
	bool m_bNiveauFin;

	CTimer m_BonusArmeTimer;
	CTimer m_BonusBouclierTimer;
	CTimer m_EnnemiTimer;

	void GererEntite ();
	void GererNiveau ();
	void InitJoueur (unsigned int uiJoueur, const char* szJoueur1, const char* szJoueur2);
	void EnregistrerScore ();

	CGameManager ();
	~CGameManager ();

public:
	//! Creation ou récupération de l'instance du gestionnaire du jeu
	static CGameManager* Instance ();
	//! Suppression de l'instance de gestionnaire du jeu si elle existe.
	static void	Kill ();
	//! Vérifie si l'instance de gestionnaire du jeu existe.
	static bool IsCreate ();

	void Update ();
	bool Init (unsigned int uiJoueur, const char* szJoueur1, const char* szJoueur2);
	void InitJoueur (CEntiteJoueur* pJoueur);
	void Tirer (EJoueur Joueur, bool bTire);
	void PositionnerJoueur (EJoueur Joueur, const glm::vec2& Position);
	bool IsNiveauFin () const {return m_bNiveauFin;}
	void ChangerNiveau () {m_bNiveauChange = true;}
};

#endif //CTRL_JEU_H

