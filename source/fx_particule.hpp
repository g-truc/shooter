//////////////////////////////////////////////////////////////////////////////
//	Shoot(r)
//////////////////////////////////////////////////////////////////////////////
//! \author Groove (http://www.g-truc.net) / Code(r) (http://www.codermag.com)
//! \date 12/05/2004
//! \file fx_particule.h
//! \brief Déclaration d'une particule
//////////////////////////////////////////////////////////////////////////////

#ifndef FX_PARTICULE_H
#define FX_PARTICULE_H

#include "util.hpp"

//! Particule utilisé pour les effets de particules
class CParticule
{
private:
	CTimer m_Temps;
	float m_fTempsVie;
	glm::vec2 m_Acceleration;
	glm::vec2 m_Position;
	glm::vec2 m_Vitesse;
	float m_fTransparence;

public:
	CParticule(
		const glm::vec2& Position,
		const glm::vec2& Vitesse, 
		const glm::vec2& Acceleration,
		float fTempsVie,
		float fTransparence);
	virtual ~CParticule ();

	//! Retourne la position de la particule
	glm::vec2 GetPosition () const;
	//! Retourne la transparence de la particule
	float GetTransparence () const;
	//! Indique si la durée de vie de la particule est dépassée
	bool GetFinVie () const;
};

#endif //FX_PARTICULE_H
