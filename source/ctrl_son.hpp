//////////////////////////////////////////////////////////////////////////////
//	Shoot(r)
//////////////////////////////////////////////////////////////////////////////
//! \author Groove (http://www.g-truc.net) / Code(r) (http://www.codermag.com)
//! \date 12/05/2004
//! \file ctrl_son.h
//! \brief Déclaration du gestionnaire de son
//////////////////////////////////////////////////////////////////////////////

#ifndef CTRL_SON_H
#define CTRL_SON_H

#include <fmod.h>
#include "util.hpp"

//! Liste des canaux utilisés pour jouer de la musique ou des sons
typedef enum
{
	CANAL_NULL = 0,
	CANAL_MUSIQUE = 0,
	CANAL_ENNEMI,
	CANAL_BONUS,
	CANAL_ARME,
	CANAL_JOUEUR1,
	CANAL_JOUEUR2,
	CANAL_MAX
} ECanaux;

//! Gestionnaire de lectures de sons
class CSoundManager
{
private:
	//! La préciseuse et unique instance de CSoundManager
	static CSoundManager *m_pSoundManager;

	//! Liste des sons, un son disponible pour chaque canal.
	FSOUND_STREAM* m_pSon[CANAL_MAX];
	//! Etat actif ou non des canaux déduiés au jeu lui-même
	bool m_bCanauxJeu;

	CSoundManager ();
	~CSoundManager ();

public:
	//! Creation ou récupération de l'instance du gestionnaire de lectures de sons 
	static CSoundManager* Instance ();
	//! Suppression de l'instance de gestionnaire de lectures de sons si elle existe.
	static void	Kill ();

	//! Lecture d'un son
	void Jouer(std::string const & Filename, ECanaux Canal, bool bLoop = false);
	//! Active les canaux de jeux
	void ActiverCanauxJeu() {m_bCanauxJeu = true;}
	//! Desactive les canaux de jeux
	void DeactiverCanauxJeu() {m_bCanauxJeu = false;}
};

#endif //CTRL_SON_H
