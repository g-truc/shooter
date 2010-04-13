//////////////////////////////////////////////////////////////////////////////
//	Shoot(r)
//////////////////////////////////////////////////////////////////////////////
//! \author Groove (http://www.g-truc.net) / Code(r) (http://www.codermag.com)
//! \date 12/05/2004
//! \file entite_joueur.cpp
//! \brief Définition d'une entité joueur
//////////////////////////////////////////////////////////////////////////////

#include "entite_joueur.hpp"
#include "entite_arme.hpp"
#include "ctrl_son.hpp"
#include "ctrl_jeu.hpp"
#include "fx_explosion.hpp"

CEntiteJoueur::CEntiteJoueur ()
{
	m_iBouclier = 100;
	m_VitesseI = m_VitesseF = glm::vec2(0.0f, 0.0f);
	m_Acceleration = glm::vec2(0.0f, 0.0f);
	m_bTire = false;
	m_uiPoint = 0;
	m_pTypeJoueur = 0;
	m_pTypeArme = 0;
	m_pFumee = 0;
	m_Timer.Init ();
}

CEntiteJoueur::~CEntiteJoueur ()
{
	// Emet le son de l'explosion de l'entité
	CSoundManager::Instance ()->Jouer (m_pTypeJoueur->GetSon (), m_pTypeJoueur->GetNiveau () == 1 ? CANAL_JOUEUR1 : CANAL_JOUEUR2);
	// Créé un effet d'explosion
	CExplosion::Create (0, 32, GetPosition (), 128, 1.0f, m_pTypeJoueur->GetCouleur ());
	// Si l'entité est lié à un effet de fumee, on indique que l'effet doit être supprimer
	if (m_pFumee)
		m_pFumee->Supprimer ();
    //CEntityManager::Instance ()->KillArmes(this);
    CGameManager::Instance ()->InitJoueur (this);
}

// Création d'une instance d'entité joueur
CEntiteJoueur* CEntiteJoueur::Create (CTypeJoueur* pTypeJoueur)
{
	// Demande au gestionnaire d'entités de créer une instance de Joueur puis on récupère une pointeur sur elle
	CEntiteJoueur *pEntite = CEntityManager::Instance ()->CreateJoueur ();
	// Vérifie que le gestionnaire a bien créer une instance. Si trop d'entités ont été créés alors le gestionnaire retourne 0
	if (!pEntite)
		return 0;

	pEntite->m_pTypeJoueur = pTypeJoueur;
	pEntite->m_PositionI = pEntite->m_PositionF = pTypeJoueur->GetPositionSpawn ();
	// Lie l'entité à un effet de fumée
	pEntite->m_pFumee = CFumee::Create (pEntite, 24, -16.f, -384.f, 0.2f, pTypeJoueur->GetCouleur ());
	return pEntite;
}

// Indique si le joueur ordonne de tirer ou non
void CEntiteJoueur::SetTirer(bool bTire)
{
	m_bTire = bTire;
}

int CEntiteJoueur::GetDommage() const
{
	return m_pTypeJoueur->GetDommage ();
}

texture * CEntiteJoueur::GetTexture() const
{
	return m_pTypeJoueur->GetTexture ();
}

float CEntiteJoueur::GetTaille () const
{
	return m_pTypeJoueur->GetTaille();
}

void CEntiteJoueur::Action()
{
	// Si le joueur n'a pas d'armes, il ne tire pas.
	if(!m_pTypeArme || !m_bTire)
		return;

	// Quand l'intervale de temps entre deux tires est écoulé alors on tire
	if(m_TireTimer.GetTime() > m_pTypeArme->GetCadence())
	{
		// Emet le son du tire
		CSoundManager::Instance()->Jouer (m_pTypeArme->GetSon(), m_pTypeJoueur->GetNiveau() == 1 ? CANAL_JOUEUR1 : CANAL_JOUEUR2);
		// Créé les entités de tire, autant que d'ancrage sur le vaisseau
		for(unsigned int i = 0; i < m_pTypeArme->GetAncrageNombre(); i++)
			CEntiteArme::Create(m_pTypeArme, this, m_pTypeArme->GetAncrageParIndex(i));

		m_TireTimer.Init();
	}
}

void CEntiteJoueur::SetArme(CTypeArme * pTypeArme)
{
	this->m_pTypeArme = pTypeArme;
}

void CEntiteJoueur::Update()
{

}

// Applique des dommages à l'entité
void CEntiteJoueur::AddDommage(int iDommage)
{
	m_iBouclier -= iDommage;
	if(m_iBouclier > 100)
		m_iBouclier = 100;
}

glm::vec3 CEntiteJoueur::GetCouleur() const
{
	glm::vec3 Couleur;
	if(m_pTypeJoueur)
		Couleur = m_pTypeJoueur->GetCouleur();
	return Couleur;
}

texture * CEntiteJoueur::GetTextureBouclier() const
{
	if(m_pTypeJoueur)
		return m_pTypeJoueur->GetTextureBouclier();
	return 0;
}

void CEntiteJoueur::AjouterPoint(unsigned int uiPoint)
{
	if(m_pTypeJoueur)
		m_pTypeJoueur->AjouterPoint(uiPoint);
}
