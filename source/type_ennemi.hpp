//////////////////////////////////////////////////////////////////////////////
//	Shoot(r)
//////////////////////////////////////////////////////////////////////////////
//! \author Groove (http://www.g-truc.net) / Code(r) (http://www.codermag.com)
//! \date 12/05/2004
//! \file type_ennemi.h
//! \brief Déclaration d'un type d'entités ennemi
//////////////////////////////////////////////////////////////////////////////

#ifndef TYPE_ENNEMI_H
#define TYPE_ENNEMI_H

#include "type.hpp"
#include "type_arme.hpp"

//! \brief Type d'entités ennemis pour des entités de types CEntiteEnnemi
//! \see CEntiteEnnemi
class CTypeEnnemi : public CType
{
private:
	CTypeArme* m_pTypeArme;
	int m_iDommage;
	unsigned int m_uiPoint;

public:
	CTypeEnnemi ();
    virtual ~CTypeEnnemi ();

	//! Retourne le type d'arme de l'ennemi
	CTypeArme* GetTypeArme () const {return m_pTypeArme;}
	//! Retourne les dommages de l'ennemi
	int GetDommage () const {return m_iDommage;}
	//! Retourne le nombre de point de l'ennemi
	unsigned int GetPoint () const {return m_uiPoint;}

	//! Indique le type d'arme de l'ennemi
	void SetTypeArme (CTypeArme* pTypeArme) {m_pTypeArme = pTypeArme;}
	//! Indique les dommages provoqués par une collision avec un type d'ennemis
	void SetDommage (int iDommage) {m_iDommage = iDommage;}
	//! Indique le nombre de point que marque un joueur en détruisant un type d'ennemis
	void SetPoint (unsigned int uiPoint) {m_uiPoint = uiPoint;}
};

#endif //TYPE_ENNEMI_H
