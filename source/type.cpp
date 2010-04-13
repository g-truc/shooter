//////////////////////////////////////////////////////////////////////////////
//	Shoot(r)
//////////////////////////////////////////////////////////////////////////////
//! \author Groove (http://www.g-truc.net) / Code(r) (http://www.codermag.com)
//! \date 12/05/2004
//! \file type.cpp
//! \brief Définition d'un type d'entités génériques, euh bon ... c'est une interface !
//////////////////////////////////////////////////////////////////////////////

#include "type.hpp"

// Au contraire des entités, les types d'entités on la responsabilité de la déallocation de la mémoire.
// Les entités se contentent d'utiliser des resourses formis par les types d'entités

CType::CType() :
	m_iBouclier(0)
{}

CType::~CType()
{}
