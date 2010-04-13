//////////////////////////////////////////////////////////////////////////////
//	Shoot(r)
//////////////////////////////////////////////////////////////////////////////
//! \author Groove (http://www.g-truc.net) / Code(r) (http://www.codermag.com)
//! \date 12/05/2004
//! \file fx_fumee.h
//! \brief Déclaration d'un générateur de particules réalisant un effet de fumée
//////////////////////////////////////////////////////////////////////////////

#ifndef FX_FUMEE_H
#define FX_FUMEE_H

#include "ctrl_particule.hpp"
#include "entite.hpp"
#include "fx.hpp"

//! Effet de fumée via particules
class CFumee : public CParticleSystem
{
	friend CFumee* CParticleManager::CreateFumee ();

private:
	entity * m_pProprietaire;
	float m_fVitesse;
	float m_fPosition;
	float m_fDureeVie;
	float m_fPrecision;

	//! Le constructeur est privé pour empécher le programmeur de créer lui même des instances
	CFumee (texture * pTexture);
	//! Le constructeur est privé pour empécher le programmeur de détrire lui même des instances
	virtual ~CFumee ();

public:
	//! Création d'une instance d'effet de particules
	static CFumee* Create(
		entity * pProprietaire, 
		float fTailleParticule, 
		float fPosition, 
		float fVitesse, 
		float fDureeVie, 
		glm::vec3 const & Couleur);

	//! Mise à jour de l'effet de particules.
	virtual void Update ();
	//! Initialisation de l'effet de particules.
	virtual void Init ();

	//! Demande la supression de l'effet de fumée
	void Supprimer ();
};

#endif //FX_FUMEE_H

