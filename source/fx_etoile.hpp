//////////////////////////////////////////////////////////////////////////////
//	Shoot(r)
//////////////////////////////////////////////////////////////////////////////
//! \author Groove (http://www.g-truc.net) / Code(r) (http://www.codermag.com)
//! \date 12/05/2004
//! \file fx_etoile.h
//! \brief Déclaration d'un générateur de particules réalisant un effet de défilement d'étoiles
//////////////////////////////////////////////////////////////////////////////

#ifndef FX_ETOILE_H
#define FX_ETOILE_H

#include "ctrl_particule.hpp"
#include "fx.hpp"

//! Effet de défilement d'étoiles via particules
class CEtoile : public CParticleSystem
{
	friend CEtoile* CParticleManager::CreateEtoile();

private:
	float m_fPosition;
	float m_fVitesseMin;
	float m_fVitesseMax;
	float m_fDureeVie;

	//! Le constructeur est privé pour empécher le programmeur de créer lui même des instances
	CEtoile(texture * pTexture);
	//! Le constructeur est privé pour empécher le programmeur de détrire lui même des instances
	virtual ~CEtoile();

public:
	//! Création d'une instance d'effet de particules
	static const CEtoile* Create();

	//! Mise à jour de l'effet de particules.
	virtual void Update();
	//! Initialisation de l'effet de particules.
	virtual void Init();
	//! Retourne la couleur des particules.
	virtual glm::vec3 GetCouleur() const;
};

#endif //FX_ETOILE_H

