//////////////////////////////////////////////////////////////////////////////
//	Shoot(r)
//////////////////////////////////////////////////////////////////////////////
//! \author Groove (http://www.g-truc.net) / Code(r) (http://www.codermag.com)
//! \date 12/05/2004
//! \file util_timer.h
//! \brief Déclaration de la classe CTimer, un gestionnaire d'écoulement du temps
//////////////////////////////////////////////////////////////////////////////

#ifndef UTIL_TIMER_H
#define UTIL_TIMER_H

#include <ctime>

//! Le gestionnaire d'écoulement du temps
class CTimer 
{
private:
	float m_fStartTime;

public:
	CTimer ();
	
	//! Initialise le timer à 0
	void Init ();
	//! Retourne le temps en second écoulé depuis la dernière initialisation
	float GetTime () const;
};

#endif //UTIL_TIMER_H
