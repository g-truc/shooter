//////////////////////////////////////////////////////////////////////////////
//	Shoot(r)
//////////////////////////////////////////////////////////////////////////////
//! \author Groove (http://www.g-truc.net) / Code(r) (http://www.codermag.com)
//! \date 12/05/2004
//! \file ctrl_entite.cpp
//! \brief Définition du gestionnaire d'entités
//////////////////////////////////////////////////////////////////////////////

#include "ctrl_entite.hpp"
#include "ctrl_fenetre.hpp"
#include "ctrl_jeu.hpp"
#include "entite_arme.hpp"
#include "entite_bonus.hpp"
#include "entite_ennemi.hpp"
#include "entite_joueur.hpp"

CEntityManager* CEntityManager::m_pInstance = 0;

// Construction du gestionnaire d'entités
CEntityManager::CEntityManager ()
{
	// On initialise le pointeur sur l'instance a 0 pour indiquer à la fonction Instance, qu'elle doit créer une instance
	m_pInstance = 0;

	// On initialise les statisques du jeu
	m_Statistique.m_uiEnnemiCree = 0;
	m_Statistique.m_uiEnnemiDetruit = 0;
	m_Statistique.m_uiEnnemiCreeTotal = 0;
	m_Statistique.m_uiEnnemiDetruitTotal = 0;

	// On initialise le tableau de pointeurs sur entités pour vérifier si de la mémoire a été alloué pour chaque entrée
	for (int i = 0; i < CTRL_ENTITE_NOMBRE; i++)
		m_pEntite[i] = 0;
}

// Destruction du gestionnaire d'entités
// On prend soit de libérer la mémoire réservée pour les entités
CEntityManager::~CEntityManager ()
{
	for (int i = 0; i < CTRL_ENTITE_NOMBRE; i++)
		if (m_pEntite[i])
			Kill (i);
}

// Creation ou récupération de l'instance du gestionnaire d'entités
// Si l'instance n'existe pas (m_pEntityManager == 0), on la créé.
CEntityManager* CEntityManager::Instance ()
{
	if (m_pInstance == 0)
		m_pInstance = new CEntityManager;
	return m_pInstance;
}

// Destruction de l'instance de gestionnaire d'entités si elle existe
void CEntityManager::Kill ()
{
	if (m_pInstance != 0)
	{
		delete m_pInstance;
		m_pInstance = 0;
	}
}

// Création d'une instance d'entité arme s'il existe un index d'entité libre
CEntiteArme* CEntityManager::CreateArme ()
{
	int iIndex = GetIndexLibre ();
	return static_cast<CEntiteArme*> (iIndex != -1 ? m_pEntite[iIndex] = new CEntiteArme () : 0);
}

// Création d'une instance d'entité bonus s'il existe un index d'entité libre
CEntiteBonus* CEntityManager::CreateBonus ()
{
	int iIndex = GetIndexLibre ();
	return static_cast<CEntiteBonus*> (iIndex != -1 ? m_pEntite[iIndex] = new CEntiteBonus () : 0);
}

// Création d'une instance d'entité ennemi s'il existe un index d'entité libre
CEntiteEnnemi* CEntityManager::CreateEnnemi ()
{
	m_Statistique.m_uiEnnemiCree++;
	int iIndex = GetIndexLibre ();
	return static_cast<CEntiteEnnemi*> (iIndex != -1 ? m_pEntite[iIndex] = new CEntiteEnnemi () : 0);
}

// Création d'une instance de joueur s'il existe un index d'entité libre
CEntiteJoueur* CEntityManager::CreateJoueur ()
{
	int iIndex = GetIndexLibre ();
	return static_cast<CEntiteJoueur*> (iIndex != -1 ? m_pEntite[iIndex] = new CEntiteJoueur () : 0);
}

// Seule cette fonction est autorisée a détruire une instance d'entités
void CEntityManager::Kill (unsigned int uiIndex)
{
    //if(m_pEntite[uiIndex]->GetType() != ENTITE_JOUEUR)
    {
	    delete m_pEntite[uiIndex];
	    m_pEntite[uiIndex] = 0;
    }
}

// Seule cette fonction est autorisée a détruire une instance d'entités
void CEntityManager::KillArmes (CEntiteJoueur* pProprietaire)
{
	for (int i = 0; i < CTRL_ENTITE_NOMBRE; i++)
    {
		if (m_pEntite[i])
			if (m_pEntite[i]->getType() == entity::ARME)
        {
            if(static_cast <CEntiteArme*> (m_pEntite[i])->GetProprietaire() == pProprietaire)
			    Kill (i);
        }
    }
/*
    if(m_pEntite[uiIndex]->GetType() != ENTITE_JOUEUR)
    {
	    delete m_pEntite[uiIndex];
	    m_pEntite[uiIndex] = 0;
    }
*/
}

// Mise à jour des statistiques de création et desctruction des entités de type ennemi
void CEntityManager::Init ()
{
	m_Statistique.m_uiEnnemiCreeTotal += m_Statistique.m_uiEnnemiCree;
	m_Statistique.m_uiEnnemiDetruitTotal += m_Statistique.m_uiEnnemiDetruit;
	m_Statistique.m_uiEnnemiCree = 0;
	m_Statistique.m_uiEnnemiDetruit = 0;
}

// Recherche d'une entrée libre dans la table des entités
int CEntityManager::GetIndexLibre () const
{
	for (int i = 0; i < CTRL_ENTITE_NOMBRE; i++)
		if (m_pEntite[i] == 0)
			return i;
	return -1;
}

// Détermine si un objet rond centrée sur Position est dans l'écran de jeu
// /!\ L'écran de jeu est plus grand que la résolution de l'écran car il faut
// une zone pour l'apparision (le spawn) des entités
bool CEntityManager::EstDansEcran (const glm::vec2& Position, float fTaille) const
{
	if (Position.x < (0 - fTaille))
		return false;
	if (Position.x > (FENETRE_LARGEUR + fTaille))
		return false;
	if (Position.y < (0 - fTaille))
		return false;
	if (Position.y > (FENETRE_HAUTEUR + fTaille + 128))
		return false;
	return true;
}

// Quand un propriétaire en détruit il est important de l'indiquer 
// à toutes les entités qu'il possède, afin qu’elle ne tente pas 
// de communiquer avec une instance qui n'existe plus ...
bool CEntityManager::AnnulerProprietaire(entity * pEntite)
{
	for (int i = 0; i < CTRL_ENTITE_NOMBRE; i++)
	{
		if (!m_pEntite[i])
			continue;

		if (m_pEntite[i]->getType() == entity::ARME)
		{
			CEntiteArme* pArme = static_cast <CEntiteArme*>(m_pEntite[i]);
			if (pArme->GetProprietaire () == pEntite)
				pArme->SetProprietaire (0);
		}
	}
	return false;
}

// La mise à jour des entités est effectué en deux temps :
// - Détection de collisions
// - Création et suppression des entités
void CEntityManager::Update ()
{
	GererCollision ();
	UpdateEntite ();
}

// Détection des collisions
void CEntityManager::GererCollision ()
{
	// On parcours tous les entités, 2 fois ! C'est donc un algorithme en O(n²) ce qui est particulièrement long
	// Il faut essayé de l'optimiser au mieux en "brisant" les boucles le plus tôt possible
	for (int i = 0; i < CTRL_ENTITE_NOMBRE; i++)
	{
		// L'entrée est null, on passe à l'entité suivante
		if (!m_pEntite[i])
			continue;
		
		// Les réactions aux collisions dépendant du type des unités
		switch (m_pEntite[i]->getType())
		{
		default:
			break;
		// L'entité 1 est un joueur
		case entity::JOUEUR:
			for (int j = 0; j < CTRL_ENTITE_NOMBRE; j++)
			{
				if (!m_pEntite[j])
					continue;

				// L'entité 2 est un ennemi
				if (m_pEntite[j]->getType() == entity::ENNEMI)
				{
					// On détermine la distance entre les entités 1 et 2
					glm::vec2 Distance = m_pEntite[i]->GetPosition () - m_pEntite[j]->GetPosition ();
					// Si la distance est trop petite alors on applique des dommages au 2 entités et on déplace l'entité 2
					if (glm::fastLength(Distance) < (m_pEntite[i]->GetTaille () + m_pEntite[j]->GetTaille () / 2) * 0.75f)
					{
						m_pEntite[i]->AddDommage (m_pEntite[j]->GetDommage ());
						m_pEntite[j]->AddDommage (m_pEntite[i]->GetDommage ());
						m_pEntite[j]->Deplacer (-glm::normalize(Distance) * 8.0f);
						break;
					}
				}
			}
			break;
		case entity::BONUS:
			for (int j = 0; j < CTRL_ENTITE_NOMBRE; j++)
			{
				if (!m_pEntite[j])
					continue;

				// Seules les joueurs peuvent entrer en collision avec bonus
				if (m_pEntite[j]->getType() == entity::JOUEUR)
				{
					// On détermine la distance entre les 2 entités
					glm::vec2 Distance = m_pEntite[i]->GetPosition () - m_pEntite[j]->GetPosition ();
					// Si la distance est suffissament petite alors le joueur prend le bonus
					if (glm::fastLength(Distance) < (m_pEntite[i]->GetTaille () / 2 + m_pEntite[j]->GetTaille ()) * 0.75f)
					{
						static_cast<CEntiteBonus *>(m_pEntite[i])->Contact (m_pEntite[j]);
						break;
					}
				}
			}
			break;
		case entity::ARME:
			for (int j = 0; j < CTRL_ENTITE_NOMBRE; j++)
			{
				if (!m_pEntite[j])
					continue;

				// Une arme peu entrée en collision avec les ennemis, les joueurs, d'autres armes
				switch(m_pEntite[j]->getType())
				{
				default:
					break;
				case entity::ENNEMI:
					// On vérifie que le propriétaire de l'arme est un joueur
					if (static_cast<CEntiteArme*> (m_pEntite[i])->GetTypeProprietaire () == entity::JOUEUR)
					{
						// On détermine la distance entre les 2 entités
						glm::vec2 Distance = m_pEntite[i]->GetPosition () - m_pEntite[j]->GetPosition ();
						if (glm::fastLength(Distance) < (m_pEntite[i]->GetTaille () / 2 + m_pEntite[j]->GetTaille () / 2) * 0.75)
						{
							// on applique les dommages
							m_pEntite[i]->AddDommage (m_pEntite[j]->GetDommage ());
							m_pEntite[j]->AddDommage (m_pEntite[i]->GetDommage ());
							// Si l'ennemi est détruit alors le joueur qui a détruit l'ennemi marque des points
							if (m_pEntite[j]->GetBouclier () <= 0)
							{
								CEntiteJoueur* pJoueur = static_cast<CEntiteJoueur*> (static_cast<CEntiteArme*> (m_pEntite[i])->GetProprietaire ());
								CEntiteEnnemi* pEnnemi = static_cast<CEntiteEnnemi*> (m_pEntite[j]);
								pJoueur->AjouterPoint (pEnnemi->GetPoint ());
							}
							break;
						}
					}
					break;
				case entity::JOUEUR:
					// On vérifie que le propriétaire de l'arme est un ennemi
					if (static_cast<CEntiteArme*> (m_pEntite[i])->GetTypeProprietaire () == entity::ENNEMI)
					{
						glm::vec2 Distance = m_pEntite[i]->GetPosition () - m_pEntite[j]->GetPosition ();
						if (glm::fastLength(Distance) < (m_pEntite[i]->GetTaille () / 2 + m_pEntite[j]->GetTaille ()) * 0.75f)
						{
							m_pEntite[i]->AddDommage (m_pEntite[j]->GetDommage ());
							m_pEntite[j]->AddDommage (m_pEntite[i]->GetDommage ());
						}
					}
					break;
				case entity::ARME:
					// Si les deux armes ont des propriétaires de types différents alors on vérifie s'il y a collision
					if (((static_cast<CEntiteArme*> (m_pEntite[i])->GetTypeProprietaire () == entity::ENNEMI)
						&& (static_cast<CEntiteArme*> (m_pEntite[j])->GetTypeProprietaire () == entity::JOUEUR))
						||((static_cast<CEntiteArme*> (m_pEntite[i])->GetTypeProprietaire () == entity::JOUEUR)
						&& (static_cast<CEntiteArme*> (m_pEntite[j])->GetTypeProprietaire () == entity::ENNEMI)))
					{
						glm::vec2 Distance = m_pEntite[i]->GetPosition () - m_pEntite[j]->GetPosition ();
						if (glm::fastLength(Distance) < (m_pEntite[i]->GetTaille () / 2 + m_pEntite[j]->GetTaille () / 2) * 0.75f)
						{
							m_pEntite[i]->AddDommage (m_pEntite[j]->GetDommage ());
							m_pEntite[j]->AddDommage (m_pEntite[i]->GetDommage ());
						}
					}
					break;
				}
			}
			break;
		}
	}
}

// Mise à jour des entités
void CEntityManager::UpdateEntite ()
{
	// La mise à jour des entités s'effectues entité par entité
	// On parcours donc toutes les entités !
	for (int i = 0; i < CTRL_ENTITE_NOMBRE; i++)
	{
		// On vérifi que le pointeur n'est pas null, c'est à dire qu'il pointe vers une instance d'entité.
		if (m_pEntite[i])
		{
			// On actualise l'entité
			m_pEntite[i]->Update();

			// Si l'entité n'est plus dans l'écran de jeu ou n'a plus de vie alors on la détruit
			if (!EstDansEcran (m_pEntite[i]->GetPosition (), m_pEntite[i]->GetTaille ())
				|| m_pEntite[i]->GetBouclier() <= 0)
			{
				switch (m_pEntite[i]->getType())
				{
				default:
					break;
				case entity::ENNEMI:
					// Indique à chaque entité dont l'ennemi est le propriétaire que son propriétaire a été détruit
					AnnulerProprietaire (m_pEntite[i]);
					m_Statistique.m_uiEnnemiDetruit++;
					break;
				case entity::JOUEUR:
					// Indique à chaque entité dont le jour est le propriétaire que son propriétaire a été détruit
					AnnulerProprietaire (m_pEntite[i]);
					CGameManager::Instance ()->InitJoueur (static_cast<CEntiteJoueur*> (m_pEntite[i]));
					break;
				case entity::BONUS:
					break;
				case entity::ARME:
					break;
				}

				Kill (i);
				// l'entité est détruite, on passe à l'entité suivante
				continue;
			}

			// Commande aux entités de type ennemi de tirer
			if (m_pEntite[i]->getType() == entity::ENNEMI)
				static_cast<CEntiteEnnemi*> (m_pEntite[i])->Action ();

			// Commande aux entités de type joueur de tirer
			if (m_pEntite[i]->getType() == entity::JOUEUR)
				static_cast<CEntiteJoueur*> (m_pEntite[i])->Action ();
		}
	}
}

// Offre un accès aux entités. On prend bien garde à ce que les entités
// ne soit pas modifier à l'aide du qualificateur const
entity const * const CEntityManager::GetEntiteParIndex (unsigned int uiIndex) const
{
	// On prend soit de vérifier la valeur de l'index afin déviter l'exception out_of_range.
	if (uiIndex < CTRL_ENTITE_NOMBRE)
		return m_pEntite[uiIndex];
	return 0;
}

// Compte le nombre d'ennemis existants.
unsigned int CEntityManager::GetEnnemiNombre () const
{
	unsigned int uiEnnemiNombre = 0;

	for (int i = 0; i < CTRL_ENTITE_NOMBRE; i++)
	{
		if (m_pEntite[i])
		{	
			if (m_pEntite[i]->getType() == entity::ENNEMI)
				uiEnnemiNombre++;
		}
	}

	return uiEnnemiNombre;
}
