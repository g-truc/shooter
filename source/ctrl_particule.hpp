//////////////////////////////////////////////////////////////////////////////
//	Shoot(r)
//////////////////////////////////////////////////////////////////////////////
//! \author Groove (http://www.g-truc.net) / Code(r) (http://www.codermag.com)
//! \date 12/05/2004
//! \file ctrl_particule.h
//! \brief Déclaration du gestionnaire de générateur de particles
//////////////////////////////////////////////////////////////////////////////

#ifndef CTRL_PARTICULE_H
#define CTRL_PARTICULE_H

#include "texture.hpp"

#define CTRL_GENERATEUR_PARTICULE_NOMBRE 32
#define FICHIER_TEXTURE_PARTICULE "image/particule.tga"

class CEtoile;
class CFumee;
class CExplosion;
class CParticleSystem;

//! Gestionnaires des générateurs de particules
class CParticleManager
{
private:
	static CParticleManager *m_pInstance;

	CParticleSystem *m_pGenParticule[CTRL_GENERATEUR_PARTICULE_NOMBRE];
	texture * m_pTexture;
	bool m_bActif;

	int GetIndexLibre () const;

	CParticleManager ();
	~CParticleManager ();

public:
	//! Creation ou récupération de l'instance du gestionnaire de générateurs de particules.
	static CParticleManager* Instance ();
	//! Suppression de l'instance de gestionnaire de générateurs de particules si elle existe.
	static void	Kill ();

	//! Accès aux générateurs de particules.
	const CParticleSystem* GetGenerateurParIndex (unsigned int uiIndex) const;

	//! Seul cette fonction permet la création d'une instance d'effet de défilement d'étoiles.
	CEtoile* CreateEtoile ();
	//! Seul cette fonction permet la création d'une instance d'effet de fumée.
	CFumee* CreateFumee ();
	//! Seul cette fonction permet la création d'une instance d'explosion.
	CExplosion* CreateExplosion ();

	//! Mise à jour du gestionnaire de générateurs de particules.
	void Update ();

	void Kill (unsigned int uiIndex);
};

#endif //CTRL_PARTICULE_H

