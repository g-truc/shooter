//////////////////////////////////////////////////////////////////////////////
//	Shoot(r)
//////////////////////////////////////////////////////////////////////////////
//! \author Groove (http://www.g-truc.net) / Code(r) (http://www.codermag.com)
//! \date 12/05/2004
//! \file entite.h
//! \brief Déclaration d'une entité générique
//////////////////////////////////////////////////////////////////////////////

#ifndef ENTITE_H
#define ENTITE_H

#include "ctrl_entite.hpp"
#include "texture.hpp"

class entity
{
public:
	enum type
	{
		NONE = 0,
		MIN = 1,
		BONUS = 1,
		ARME,
		ENNEMI,
		JOUEUR,
		MAX
	};

public:
	entity();
	virtual ~entity (){}

	//! Retourne la texture de l'entité
	virtual texture * GetTexture() const = 0;
	//! Retourne les dommages de l'entités
	virtual int GetDommage() const = 0;
	//! Met à jour l'entité
	virtual void Update();
	//! Recupère le type de l'entité
	virtual type getType() const = 0;
	//! Retourne la taille de l'entité
	virtual float GetTaille() const = 0;
	//! Ajout des dommages à l'entité
	virtual void AddDommage(int iDommage);

	//! Modification de la position de l'entité
	void Deplacer(const glm::vec2& Deplacement) {m_PositionI += Deplacement;}
	//! Retourne la position de l'entité
	glm::vec2 GetPosition() const {return m_PositionF;}
	//! Positionne l'entité à la position indiquée par Position
	void SetPosition(const glm::vec2& Position) {m_PositionF = Position;}
	//! Retour la valeur de l'entité
	int GetBouclier() const {return m_iBouclier;}
	//! Inicialisation du bouclier de l'entité
	void SetBouclier(int iBouclier) {m_iBouclier = iBouclier;}

protected:
	//! Permet de déterminer si une entité en dans l'écran de jeu ou non
	bool EstDansEcran(glm::vec2 const & Position, float fRayon) const;

	CTimer m_Timer;
	glm::vec2 m_PositionI, m_PositionF;
	glm::vec2 m_VitesseI, m_VitesseF;
	glm::vec2 m_Acceleration;
	int m_iBouclier;
	unsigned int m_uiPoint;
};

#endif // ENTITE_H

