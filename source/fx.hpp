//////////////////////////////////////////////////////////////////////////////
//	Shoot(r)
//////////////////////////////////////////////////////////////////////////////
//! \author Groove (http://www.g-truc.net) / Code(r) (http://www.codermag.com)
//! \date 12/05/2004
//! \file fx.h
//! \brief Déclaration d'un générateur de particules génériques
//////////////////////////////////////////////////////////////////////////////

#ifndef FX_H
#define FX_H

#include "ctrl_particule.hpp"
#include "entite.hpp"
#include "fx_particule.hpp"
#include "texture.hpp"

#define PARTICULE_NOMBRE 128

//! Classe générique de générateurs de particules
class CParticleSystem
{
	friend void CParticleManager::Kill (unsigned int uiIndex);

protected:
	entity * m_pProprietaire;
	CParticule *m_pParticule[PARTICULE_NOMBRE];
	float m_fTailleParticule;
	texture * m_pTexture;
	bool m_bSupprime;
	CTimer m_Timer;
	glm::vec3 m_Couleur;

	//! Recherche d'un emplacement libre dans la table des particules.
	int GetIndexLibre () const;
	//! Vérifie si le générateur de particules est dans l'écran.
	bool EstDansEcran (const glm::vec2& Position, float fRayon) const;

	CParticleSystem ();
	virtual ~CParticleSystem ();

public:
	//! Offre un accès au tableau de pointeur sur particule. Ce type d'opération
	//! est une source potencielle de risques. 
	//! Pour les limités, nous prenons soin de déclarer "const" le type de 
	//! sortie afin de garantir qu'à l'utilisation, ce pointeur et sa valeur
	//! ne seront pas modifié.
	const CParticule* const GetParticuleParIndex (unsigned int uiIndex) const;

	//! Mise à jour du générateur de particules.
	virtual void Update () = 0;
	//! Initialisation du générateur de particules.
	virtual void Init () = 0;
	//! Retourne la couleur des particules.
	virtual glm::vec3 GetCouleur () const {return m_Couleur;}
	//! Faut t'il supprimer le générateur ?
	bool GetSupprime () const {return m_bSupprime;}
	//! Retourne la texture du générateur de particules.
	texture * GetTexture () const {return m_pTexture;}
	//! Retourne la taille des particules.
	float GetTaille () const {return m_fTailleParticule;}
};

#endif //FX_H
