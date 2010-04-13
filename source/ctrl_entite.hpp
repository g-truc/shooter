//////////////////////////////////////////////////////////////////////////////
//	Shoot(r)
//////////////////////////////////////////////////////////////////////////////
//! \author Groove (http://www.g-truc.net) / Code(r) (http://www.codermag.com)
//! \date 25/05/2004
//! \file ctrl_entite.h
//! \brief Déclaration du gestionnaire d'entités
//////////////////////////////////////////////////////////////////////////////

#ifndef CTRL_ENTITE_H
#define CTRL_ENTITE_H

#include "util.hpp"

//! Le jeu est limité à CTRL_ENTITE_NOMBRE entités simultanément.
//! Limité le nombre d'entités permet notamment de fixer un plafond aux 
//! ressources réquisent par le jeu, tout en améliorant les performences.
//! En effet, on aurait très bien plus ne pas fixer de limite en 
//! utilisant une liste chainé ou un verteur de données cependant dans 
//! les deux cas, on aura notez une grosse perte de performance du 
//! principalement à des coups de réallocation dynamique (transparent si
//! l'on utilise la STL avec std::list ou stl::vector)
std::size_t const CTRL_ENTITE_NOMBRE = 96;

class entity;
class CEntiteArme;
class CEntiteBonus;
class CEntiteEnnemi;
class CEntiteJoueur;

//! \brief Cette structure permet de sauvegarder le nombre 
//! d'énnemi créé et détruit total et pour le niveau. 
//! on utilise cette structure pour déterminer la fin d'une partie
typedef struct
{
	unsigned int m_uiEnnemiCree;
	unsigned int m_uiEnnemiDetruit;
	unsigned int m_uiEnnemiCreeTotal;
	unsigned int m_uiEnnemiDetruitTotal;
} SStatistique;

//! Le gestionnaire d'entités
class CEntityManager
{
private:
	static CEntityManager *m_pInstance;

	//! Notre tableau de pointeur vers des entités.
	//! On utilise des pointeurs sur entités afin de profiter du mécanisme
	//! de l'héritage avec des fonctions virtuelles. En clair, les instances
	//! pointer par ce tableau, pointe vers des instances de classes dérivés
	//! de la classe CEntite. On utilise des fonctions virtuelles déclarés
	//! dans la classe CEntite pour accéder aux fonctions de même prototype
	//! déclaré dans les classes dérivés de CEntite.
	entity * m_pEntite[CTRL_ENTITE_NOMBRE];

	//! Nombre d'ennemis créés et détruits dans le niveau et la partie
	SStatistique m_Statistique;

	//! Recherche d'un emplacement libre dans la table des entités
	int GetIndexLibre () const;
	//! Permet de déterminer si une entité en dans l'écran de jeu ou non
	bool EstDansEcran (const glm::vec2& Position, float fTaille) const;
	//! Mise à jours de entités
	void UpdateEntite ();
	//! Gestion des collisions entre entités : Il y a t'il des entités qui se chevauchent et comment réagir
	void GererCollision ();
	//! Annule le propriétaire pEntite, de toutes entités qui possédent un propriétaire
	bool AnnulerProprietaire (entity * pEntite);

	//! Constructeur
	CEntityManager ();
	//! Desctructeur
	~CEntityManager ();

public:
	//! Creation ou récupération de l'instance de gestionnaire d'entités.
	static CEntityManager* Instance ();
	//! Suppression de l'instance de gestionnaire d'entités si elle existe.
	static void	Kill ();

	//! Création d'une instance d'entité arme.
	//! Si le nombre maximum, l'entité est atteind, ces fonctions retournent 0. Il faut donc contrôler son retour.
	CEntiteArme* CreateArme ();
	//! Création d'une instance d'entité bonus.
	//! Si le nombre maximum, l'entité est atteind, ces fonctions retournent 0. Il faut donc contrôler son retour.
	CEntiteBonus* CreateBonus ();
	//! Création d'une instance d'entité ennemi.
	//! Si le nombre maximum, l'entité est atteind, ces fonctions retournent 0. Il faut donc contrôler son retour.
	CEntiteEnnemi* CreateEnnemi ();
	//! Création d'une instance d'entité joueur.
	//! Si le nombre maximum, l'entité est atteind, ces fonctions retournent 0. Il faut donc contrôler son retour.
	CEntiteJoueur* CreateJoueur ();

	//! Offre un accès au tableau de pointeur sur entité. Ce type d'opération
	//! est une source potencielle de risques. 
	//! Pour les limités, nous prenons soin de déclarer "const" le type de 
	//! sortie afin de garantir qu'à l'utilisation, ce pointeur et sa valeur
	//! ne seront pas modifié.
	entity const * const GetEntiteParIndex (unsigned int uiIndex) const;
	//! Offre un accès aux statistiques du projet.
	const SStatistique & GetStatistique () const {return m_Statistique;}

	//! Compte le nombre d'ennemis existants.
	unsigned int GetEnnemiNombre () const;

    //! Supression des armes d'un joueur.
    void KillArmes (CEntiteJoueur * pProprietaire);

	//! Initialisation du gestionnaire d'entité.
	void Init ();
	//! Mise à jour du gestionnaire d'entité.
	void Update ();

	void Kill (unsigned int uiIndex);
};

#endif //CTRL_ENTITE_H

