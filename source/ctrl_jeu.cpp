//////////////////////////////////////////////////////////////////////////////
//	Shoot(r)
//////////////////////////////////////////////////////////////////////////////
//! \author Groove (http://www.g-truc.net) / Code(r) (http://www.codermag.com)
//! \date 14/05/2004
//! \file ctrl_jeu.cpp
//! \brief Définition du coeur du projet, CGameManager englobe les niveaux, indique quand créer les entités, détermine le début et la fin d'une partie.
//////////////////////////////////////////////////////////////////////////////

#include "affichage.hpp"
#include "ctrl_fenetre.hpp"
#include "ctrl_jeu.hpp"
#include "ctrl_particule.hpp"
#include "ctrl_son.hpp"
#include "ctrl_entite.hpp"
#include "entite_arme.hpp"
#include "entite_bonus.hpp"
#include "entite_ennemi.hpp"
#include "entite_joueur.hpp"
#include "xml_score.hpp"
#include "fx_etoile.hpp"
#include "fx_explosion.hpp"
#include "texture_manager.hpp"

CGameManager* CGameManager::m_pGameManager = 0;

// On initialise tous les pointeurs à 0. 
// Ce réflexe permet de vérifier si de la mémoire a été alloué.
CGameManager::CGameManager ()
{
	m_pGameManager = 0;
	m_pJoueur1 = 0;
	m_pJoueur2 = 0;
	m_uiNiveau = 0;
	m_bNiveauFin = false;
	m_bNiveauChange = false;
	m_Etat = ETAT_JEU_JEU;
	m_TypeJoueur = new CTypeJoueur [TYPE_JOUEUR_NOMBRE];

	// On quite le jeu donc on desactive les canaux utilisés pour le jeu
	CSoundManager::Instance () ->ActiverCanauxJeu ();
}

CGameManager::~CGameManager ()
{
	// On quite le jeu donc on desactive les canaux utilisés pour le jeu
	CSoundManager::Instance ()->DeactiverCanauxJeu ();

	// Libération de la mémoire réservée pour le contrôleur d'entités.
	CEntityManager::Kill ();
	
	// Libération de la mémoire réservée par le moteur de particules.
	CParticleManager::Kill ();

	delete [] m_TypeJoueur;
	m_TypeJoueur = 0;
}

// Creation ou récupération de l'instance du gestionnaire de jeu.
// Si l'instance n'existe pas (m_pGameManager == 0), on la créé.
CGameManager* CGameManager::Instance ()
{
	if (m_pGameManager == 0)
		m_pGameManager = new CGameManager;
	return m_pGameManager;
}

// Suppression de l'instance du gestionnaire de jeu si elle existe.
void CGameManager::Kill ()
{
	if (m_pGameManager != 0)
	{
		delete m_pGameManager;
		m_pGameManager = 0;
	}
}

bool CGameManager::IsCreate ()
{
	return m_pGameManager != 0;
}

void CGameManager::EnregistrerScore ()
{
	SScore Score;

	// Chargement du fichier XML contenant les scores
	CXMLScore XMLScore;
	XMLScore.Charger (FICHIER_XML_SCORE);

	// Création d'un résultat pour le joueur 1
	SJoueur Joueur1;
	Joueur1.m_szNom = m_TypeJoueur[0].GetNom ();
	Joueur1.m_uiScore = m_TypeJoueur[0].GetScore ();
	Joueur1.m_uiNiveau = m_TypeJoueur[0].GetNiveau ();
	Score.m_Joueur.push_back (Joueur1);

	// Création d'un résultat pour le joueur 2, si c'est une partie pour 2 joueurs
	if (m_uiTypeJoueur == 2)
	{
		SJoueur Joueur2;
		Joueur2.m_szNom = m_TypeJoueur[1].GetNom ();
		Joueur2.m_uiScore = m_TypeJoueur[1].GetScore ();
		Joueur2.m_uiNiveau = m_TypeJoueur[1].GetNiveau ();
		Score.m_Joueur.push_back (Joueur2);
	}

	// Enregistrement du score
	XMLScore.AjouterScore (Score);
	XMLScore.Sauver ();
}

void CGameManager::GererNiveau ()
{
	CEntityManager *pControleurEntite = CEntityManager::Instance ();

	// Détermine s'il y a défaite
	if (m_pJoueur1 == 0 && m_pJoueur2 == 0 && m_Etat != ETAT_JEU_DEFAITE)
	{
		m_Etat = ETAT_JEU_DEFAITE;
		EnregistrerScore ();
	}

	if (m_NiveauTemps.GetTime () + TEMPS_ENTRE_NIVEAU > m_XMLNiveaux.GetTemps (m_uiNiveau) && pControleurEntite->GetEnnemiNombre () == 0)
	{
		if (m_uiNiveau == m_XMLNiveaux.GetNiveauNombre () - 1)
		{
			m_bNiveauChange = true;
		}
		else
		{
			m_Etat = ETAT_JEU_FIN_NIVEAU;
			m_bNiveauFin = true;
		}
	}

	// Changement de niveau
	if (m_bNiveauChange) 
	{
		m_Etat = ETAT_JEU_JEU;
		m_bNiveauFin = false;
		m_bNiveauChange = false;
		m_NiveauTemps.Init ();
		m_uiNiveau++;
		pControleurEntite->Init ();
		// Détermine s'il y a victoire
		if (m_uiNiveau >= m_XMLNiveaux.GetNiveauNombre ())
		{
			// Indique pour chaque joueur de la partie le niveau atteint (le dernier)
			m_uiNiveau = m_XMLNiveaux.GetNiveauNombre () - 1;
			m_Etat = ETAT_JEU_VICTOIRE;
			m_TypeJoueur[0].SetNiveau (m_uiNiveau + 1);
			if (m_uiTypeJoueur == 2)
				m_TypeJoueur[1].SetNiveau (m_uiNiveau + 1);
			// Enregistrement des scores obtenus par les joueurs
			EnregistrerScore ();

			// Le bouquet final !
			for (int i = 0; i < 8; i++)
			{
				glm::vec3 Couleur (glm::compRand3(0.f, 1.f));
				CExplosion::Create (
					0, 32, 
					glm::compRand2(glm::vec2(0), glm::vec2(FENETRE_LARGEUR, FENETRE_HAUTEUR)), 
					glm::compRand1(100, 200), 
					glm::compRand1(1.f, 4.f), 
					Couleur);
			}
		}
	}
}

void CGameManager::Update ()
{
	// S'il n'y a niveau victoire ni défaire, on met à jour le niveau, les entités et les particules
	if (m_Etat != ETAT_JEU_DEFAITE && m_Etat != ETAT_JEU_VICTOIRE)
		GererNiveau ();

	// Création des entités
	GererEntite ();
	// Mise à jour des entités
	CEntityManager::Instance ()->Update ();
	// Mise à jour des effets de particules
	CParticleManager::Instance ()->Update ();

	CAffichage *pAffichage = CAffichage::Instance ();
	pAffichage->Debut ();
	// On affiche les particules
	pAffichage->Particule ();
	// On affciche les entités
	pAffichage->Entite ();

	// On affiche le HUD, suivant l'état du jeu et le nombre de joueurs dans la partie
	switch (m_Etat)
	{
	default:
		break;
	case ETAT_JEU_JEU:
		if (m_uiTypeJoueur == 2)
			pAffichage->HUDJeu (m_uiNiveau, m_TypeJoueur[0].GetScore (), m_TypeJoueur[0].GetNom (), m_TypeJoueur[1].GetScore (), m_TypeJoueur[1].GetNom ());
		else
			pAffichage->HUDJeu (m_uiNiveau, m_TypeJoueur[0].GetScore (), m_TypeJoueur[0].GetNom ());
		break;
	case ETAT_JEU_VICTOIRE:
		if (m_uiTypeJoueur == 2)
			pAffichage->HUDVictoire (m_TypeJoueur[0].GetScore (), m_TypeJoueur[0].GetNom (), m_TypeJoueur[1].GetScore (), m_TypeJoueur[1].GetNom ());
		else
			pAffichage->HUDVictoire (m_TypeJoueur[0].GetScore (), m_TypeJoueur[0].GetNom ());
		break;
	case ETAT_JEU_DEFAITE:
		if (m_uiTypeJoueur == 2)
			pAffichage->HUDDefaite (m_TypeJoueur[0].GetScore (), m_TypeJoueur[0].GetNom (), m_TypeJoueur[1].GetScore (), m_TypeJoueur[1].GetNom ());
		else
			pAffichage->HUDDefaite (m_TypeJoueur[0].GetScore (), m_TypeJoueur[0].GetNom ());
		break;
	case ETAT_JEU_FIN_NIVEAU:
		if (m_uiTypeJoueur == 2)
			pAffichage->HUDFinNiveau (m_uiNiveau, m_TypeJoueur[0].GetScore (), m_TypeJoueur[0].GetNom (), m_TypeJoueur[1].GetScore (), m_TypeJoueur[1].GetNom ());
		else
			pAffichage->HUDFinNiveau (m_uiNiveau, m_TypeJoueur[0].GetScore (), m_TypeJoueur[0].GetNom ());
		break;
	}
	pAffichage->Fin ();
}

void CGameManager::GererEntite ()
{
	if (m_NiveauTemps.GetTime () < m_XMLNiveaux.GetTemps (m_uiNiveau) && m_Etat == ETAT_JEU_JEU)
	//if (pEntityManager->GetStatistique ().m_uiEnnemiCree < m_XMLNiveaux.GetEnnemiNombre (m_uiNiveau) && m_bNiveau)
	{
		// Ajout des ennemis
		// Quand l'intervale de temps entre la création de deux ennemis défini dans le niveau est écoulé, alors on créé une entité ennemi.
		if (m_EnnemiTimer.GetTime () > m_XMLNiveaux.GetEnnemiFrequence (m_uiNiveau))
		{
			m_EnnemiTimer.Init ();
			CEntiteEnnemi::Create (m_XMLNiveaux.GetTypeEnnemi (m_uiNiveau), glm::vec2(glm::compRand1(64, FENETRE_LARGEUR - 64), FENETRE_HAUTEUR + 96));
		}

		// Ajout des ennemis
		// Quand l'intervale de temps entre la création de deux bonus d'arme défini dans le niveau est écoulé, alors on créé une entité bonus d'arme.
		if (m_BonusArmeTimer.GetTime () > m_XMLNiveaux.GetBonusArmeFrequence (m_uiNiveau))
		{
			m_BonusArmeTimer.Init ();
			CEntiteBonus::Create (m_XMLNiveaux.GetTypeBonusArme (m_uiNiveau), glm::vec2(glm::compRand1(64, FENETRE_LARGEUR - 64), FENETRE_HAUTEUR + 96));
		}

		// Ajout des ennemis
		// Quand l'intervale de temps entre la création de deux bonus de bouclier défini dans le niveau est écoulé, alors on créé une entité bonus de bouclier.
		if (m_BonusBouclierTimer.GetTime () > m_XMLNiveaux.GetBonusBouclierFrequence (m_uiNiveau))
		{
			m_BonusBouclierTimer.Init ();
			CEntiteBonus::Create (m_XMLNiveaux.GetTypeBonusBouclier (m_uiNiveau), glm::vec2(glm::compRand1(64, FENETRE_LARGEUR - 64), FENETRE_HAUTEUR + 96));
		}
	}
}

bool CGameManager::Init (unsigned int uiJoueur, const char* szJoueur1, const char* szJoueur2)
{
	// Chargement des niveaux, en cas d'echec on renvoie une erreur.
	if(!m_XMLNiveaux.Charger((DIRECTORY + FICHIER_XML_NIVEAU).c_str()))
		return false;

	// Creation de l'instance de l'entity manager
	CEntityManager::Instance();
	// Creation de l'instance du particle manager
	CParticleManager::Instance();

	// Initialisation des joueurs, création des entités joueurs
	InitJoueur(uiJoueur, szJoueur1, szJoueur2);

	// Création d'une instance d'effet de défilement d'étoiles
	CEtoile::Create();

	// Initialisation des timers
	m_EnnemiTimer.Init();
	m_BonusArmeTimer.Init();
	m_BonusBouclierTimer.Init();
	m_NiveauTemps.Init();

	return true;
}

// Création des entités du joueur
void CGameManager::InitJoueur(unsigned int uiJoueur, const char* szJoueur1, const char* szJoueur2)
{
	m_uiTypeJoueur = uiJoueur;

	// Configuration du joueur 1
	m_TypeJoueur[0].SetNumero(1);
	m_TypeJoueur[0].SetTexture(textureManager::instance()->createTexture2D(FIFHIER_TEXTURE_VAISSEAU));
	m_TypeJoueur[0].SetTextureBouclier(textureManager::instance()->createTexture2D(FIFHIER_TEXTURE_BOUCLIER));
	m_TypeJoueur[0].SetCouleur(glm::vec3(0.5f, 0.5f, 1.0f));
	m_TypeJoueur[0].SetDommage(5);
	m_TypeJoueur[0].SetTaille(32);
	m_TypeJoueur[0].SetSon(FICHIER_SON_EXPLOSION);
	m_TypeJoueur[0].SetNom(szJoueur1);
	m_TypeJoueur[0].SetPositionSpawn(glm::vec2(FENETRE_LARGEUR * 1.f / 3.f, FENETRE_HAUTEUR / 6.f));

	// Configuration du joueur 2
	m_TypeJoueur[1].SetNumero(2);
	m_TypeJoueur[1].SetTexture(textureManager::instance()->createTexture2D(FIFHIER_TEXTURE_VAISSEAU));
	m_TypeJoueur[1].SetTextureBouclier(textureManager::instance()->createTexture2D(FIFHIER_TEXTURE_BOUCLIER));
	m_TypeJoueur[1].SetCouleur(glm::vec3(1.0f, 0.5f, 0.5f));
	m_TypeJoueur[1].SetDommage(5);
	m_TypeJoueur[1].SetTaille(32);
	m_TypeJoueur[1].SetSon(FICHIER_SON_EXPLOSION);
	m_TypeJoueur[1].SetNom(szJoueur2);
	m_TypeJoueur[1].SetPositionSpawn(glm::vec2(FENETRE_LARGEUR * 2.f / 3.f, FENETRE_HAUTEUR / 6.f));

	// Création de l'entité du joueur 1
	m_pJoueur1 = CEntiteJoueur::Create(&m_TypeJoueur[0]);
	m_pJoueur1->SetArme(m_XMLNiveaux.GetTypeBonusArme(0)->GetArme());

	if(m_uiTypeJoueur == 2)
	{
		// Création de l'entité du joueur 2
		m_pJoueur2 = CEntiteJoueur::Create(&m_TypeJoueur[1]);
		m_pJoueur2->SetArme(m_XMLNiveaux.GetTypeBonusArme (0)->GetArme());
	}
}

// Commande de tire des joueurs
void CGameManager::Tirer (EJoueur Joueur, bool bTire)
{
	switch (Joueur)
	{
	default:
		break;
	case JOUEUR_1:
		if (m_pJoueur1)
			m_pJoueur1->SetTirer (bTire);
		break;
	case JOUEUR_2:
		if (m_pJoueur2)
			m_pJoueur2->SetTirer (bTire);
		break;
	}
}

// Commande de positionnement des joueurs
void CGameManager::PositionnerJoueur(EJoueur Joueur, const glm::vec2& Position)
{
	switch(Joueur)
	{
	default:
		break;
	case JOUEUR_1:
		if(m_pJoueur1)
			m_pJoueur1->SetPosition(Position);
		break;
	case JOUEUR_2:
		if(m_pJoueur2)
			m_pJoueur2->SetPosition(Position);
		break;
	}
}

// Initialisation des joueurs quand les entités joueurs sont détruites
void CGameManager::InitJoueur(CEntiteJoueur* pJoueur)
{
	if(m_pJoueur1 == pJoueur)
	{
		// On indique le niveau atteint pas le joueur 1
		m_TypeJoueur[0].SetNiveau(m_uiNiveau + 1);
		m_pJoueur1 = 0;
	}
	if(m_pJoueur2 == pJoueur)
	{
		// On indique le niveau atteint pas le joueur 2
		if(m_uiTypeJoueur == 2)
			m_TypeJoueur[1].SetNiveau(m_uiNiveau + 1);
		m_pJoueur2 = 0;
	}
}

