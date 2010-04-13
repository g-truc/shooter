//////////////////////////////////////////////////////////////////////////////
//	Shoot(r)
//////////////////////////////////////////////////////////////////////////////
//! \author Groove (http://www.g-truc.net) / Code(r) (http://www.codermag.com)
//! \date 26/05/2004
//! \file affichage.cpp
//! \brief Définition de la classe CAffichage pour le menu, le HUD, les particules
//////////////////////////////////////////////////////////////////////////////

#include "affichage.hpp"
#include "ctrl_entite.hpp"
#include "ctrl_particule.hpp"
#include "ctrl_fenetre.hpp"
#include "entite_arme.hpp"
#include "entite_bonus.hpp"
#include "entite_ennemi.hpp"
#include "entite_joueur.hpp"
#include "xml_score.hpp"
#include "window_sdl.hpp"
#include <sstream>

std::string const FICHIER_TEXTURE_FONT("image/font.tga");

std::string const MENU_TEXTE_REPRENDRE("Reprendre la partie");
std::string const MENU_TEXTE_COMMENCER("Commencer une partie");
std::string const MENU_TEXTE_SCORES("Voir les scores");
std::string const MENU_TEXTE_QUITTER("Quitter");

CAffichage* CAffichage::m_pInstance = 0;

CAffichage::CAffichage ()
{
	m_pInstance = 0;
	m_iFpsCompte = 0;
	m_iFpsNiveau = 0;
	m_iFps = 0;
	m_FpsTimer.Init ();

	// On récupère la liste des extensions supportés par les drivers de la carte graphique.
    char* szExtList = (char*) glGetString (GL_EXTENSIONS);

	// Pour ce jeu, nous utilisons l'extension GL_ARB_point_sprite.
	// Pour nous assurer qu'elle est supporté nous vérifions que la chaîne "GL_ARB_point_sprite"
	// est présente dans la chaîne récupérée avec glGetString (GL_EXTENSIONS)
    if (!strstr (szExtList, "GL_ARB_point_sprite"))
	{
		// L'extension GL_ARB_point_sprite n'est pas supporté 
		m_bPointSprite = false;
		fprintf (stderr, "L'extension OpenGL 'GL_ARB_point_sprite', n'est pas supporté ...\n");
		fprintf (stderr, "Veuillez installer les derniers pilotes de votre carte graphique.\n");
	}
	else
	{
		// L'extension GL_ARB_point_sprite est supporté 
		m_bPointSprite = true;
	}

	// Couleur utiliser quand le tampon chromatique est vidé
	glClearColor (0.0f, 0.0f, 0.1f, 0.0f);
	glShadeModel (GL_FLAT);

	Debut ();
	Fin ();

	// Charge la texture utilisé pour l'affichage des caractères
	m_Font.ChargerTexture(DIRECTORY + FICHIER_TEXTURE_FONT);
}

// Creation ou récupération de l'instance de l'affichage.
// Si l'instance n'existe pas (m_pRenderingManager == 0), on la créé.
CAffichage* CAffichage::Instance ()
{
	if (m_pInstance == 0)
		m_pInstance = new CAffichage;
	return m_pInstance;
}

void CAffichage::Kill ()
{
	// On detruit le rendering manager
	if (m_pInstance != 0)
	{
		delete m_pInstance;
		m_pInstance = 0;
	}
}

/*
Entités
*/

// Suivant le support ou non de l'extension GL_ARB_point_sprite, on choisir le mode de rendu
void CAffichage::Entite () const
{
	if (m_bPointSprite)
		EntitePoint  ();
	else
		EntiteQuad  ();
}

// Les entités sont affichés sous forme de poins si l'extenion GL_ARB_point_sprtie est supporté
void CAffichage::EntitePoint () const
{
	const CEntityManager *pControleurEntite = CEntityManager::Instance ();

	// Activation des textures
	glEnable (GL_TEXTURE_2D);
	// Activation du remplacement des points par des textures
	glEnable (GL_POINT_SPRITE_ARB);
	// Activation de la tranparence
	glEnable (GL_BLEND);
	// Activation du masquage des faces arrières
	glEnable (GL_CULL_FACE);

	// On centre la texture sur le point
	glTexEnvf (GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_TRUE);
	// Défini le mode de transparence
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// On parcours toutes les entités pour tout les affichés mais par type.
	// Comme tous les dessins sont affichés les uns au dessus des autres,
	// alors au défini l'ordre d'affichage suivante le type des entités
	// Pour modifier l'ordre, il suffit de modifier l'enumération EEntiteType
	for (int EntiteType = entity::MIN; EntiteType < entity::MAX; EntiteType++)
	{
		for (int i = 0; i < CTRL_ENTITE_NOMBRE; i++)
		{
			// On récupère l'entité d'index i dans la table des entités
			entity const * pEntite = pControleurEntite->GetEntiteParIndex (i);
			if (pEntite)
			{
				if (pEntite->getType() == EntiteType)
				{
					// On indique que l'on utilise la texture de l'entité
					//glBindTexture (GL_TEXTURE_2D, pEntite->GetTexture ());
					pEntite->GetTexture()->bind();
					// On indique la taille de l'entité
					glPointSize(pEntite->GetTaille());
					// On dessin l'entité sous forme de points
					glBegin(GL_POINTS);
						glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
						glVertex2fv(&pEntite->GetPosition()[0]);
					glEnd();

					// Dessin le bouclier
					if(pEntite->getType() == entity::JOUEUR)
					{
						CEntiteJoueur const * pJoueur = static_cast<const CEntiteJoueur*>(pEntite);
						glBlendFunc(GL_SRC_ALPHA, GL_ONE);
						//glBindTexture (GL_TEXTURE_2D, pJoueur->GetTextureBouclier ());
						pJoueur->GetTextureBouclier()->bind();
						glPointSize(pEntite->GetTaille() * BOUCLIER_TAILLE);
						glBegin(GL_POINTS);
							glColor4f(pJoueur->GetCouleur().r, pJoueur->GetCouleur().g, pJoueur->GetCouleur().b, pJoueur->GetBouclier() / 100.f);//200.f + 0.5f);
							glVertex2fv(&pJoueur->GetPosition()[0]);
						glEnd();
						glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
					}
				}
			}
		}
	}

	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);
	glDisable(GL_POINT_SPRITE_ARB);
	glDisable(GL_TEXTURE_2D);
}

// Les entités sont affichés sous forme de quadrangles si l'extenion GL_ARB_point_sprtie n'est pas supporté
void CAffichage::EntiteQuad() const
{
	CEntityManager const * pControleurEntite = CEntityManager::Instance();

	// Activation des textures
	glEnable(GL_TEXTURE_2D);
	// Activation de la tranparence
	glEnable(GL_BLEND);
	// Activation du masquage des faces arrières
	glEnable(GL_CULL_FACE);

	// Défini le mode de transparence
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// On parcours toutes les entités pour tout les affichés mais par type.
	// Comme tous les dessins sont affichés les uns au dessus des autres,
	// alors au défini l'ordre d'affichage suivante le type des entités
	// Pour modifier l'ordre, il suffit de modifier l'enumération EEntiteType
	for(int EntiteType = entity::MIN; EntiteType < entity::MAX; EntiteType++)
	{
		for(int i = 0; i < CTRL_ENTITE_NOMBRE; i++)
		{
			// On récupère l'entité d'index i dans la table des entités
			entity const * pEntite = pControleurEntite->GetEntiteParIndex(i);
			if(pEntite)
			{
				if(pEntite->getType() == EntiteType)
				{
					glm::vec2 Position = pEntite->GetPosition();
					float fTaille = pEntite->GetTaille() / 2;
					// On indique que l'on utilise la texture de l'entité
					//glBindTexture (GL_TEXTURE_2D, pEntite->GetTexture ());
					pEntite->GetTexture ()->bind();
					// On dessin l'entité sous forme de quadrangle
					glBegin(GL_QUADS);
						glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
						glTexCoord2f(1, 1); glVertex2fv(&(Position + glm::vec2(-fTaille, -fTaille))[0]);
						glTexCoord2f(0, 1); glVertex2fv(&(Position + glm::vec2(fTaille, -fTaille))[0]);
						glTexCoord2f(0, 0); glVertex2fv(&(Position + glm::vec2(fTaille, fTaille))[0]);
						glTexCoord2f(1, 0); glVertex2fv(&(Position + glm::vec2(-fTaille, fTaille))[0]);
					glEnd();

					fTaille *= BOUCLIER_TAILLE;
					if(pEntite->getType() == entity::JOUEUR)
					{
						CEntiteJoueur const * pJoueur = static_cast<CEntiteJoueur const *>(pEntite);
						glBlendFunc (GL_SRC_ALPHA, GL_ONE);
						//glBindTexture (GL_TEXTURE_2D, pJoueur->GetTextureBouclier ());
						pJoueur->GetTextureBouclier()->bind();
						glBegin(GL_QUADS);
							glColor4f(pJoueur->GetCouleur().r, pJoueur->GetCouleur().g, pJoueur->GetCouleur ().b, pJoueur->GetBouclier() / 100.f);//200.f + 0.5f);
							glTexCoord2f(1, 1); glVertex2fv(&(Position + glm::vec2(-fTaille, -fTaille))[0]);
							glTexCoord2f(0, 1); glVertex2fv(&(Position + glm::vec2(fTaille, -fTaille))[0]);
							glTexCoord2f(0, 0); glVertex2fv(&(Position + glm::vec2(fTaille, fTaille))[0]);
							glTexCoord2f(1, 0); glVertex2fv(&(Position + glm::vec2(-fTaille, fTaille))[0]);
						glEnd();
						glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
					}
				}
			}
		}
	}

	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
}

/*
Particule
*/

// Suivant le support ou non de l'extension GL_ARB_point_sprite, on choisir le mode de rendu
void CAffichage::Particule() const
{
	if(m_bPointSprite)
		ParticulePoint();
	else
		ParticuleQuad();
}

// Les entités sont affichés sous forme de poins si l'extenion GL_ARB_point_sprtie est supporté
void CAffichage::ParticulePoint() const
{
	CParticleManager const * pControleurParticule = CParticleManager::Instance();

	// Activation des textures
	glEnable(GL_TEXTURE_2D);
	// Activation du remplacement des points par des textures
	glEnable(GL_POINT_SPRITE_ARB);
	// Activation de la transparence
	glEnable(GL_BLEND);
	// Activation du masquage des faces arrières
	glEnable(GL_CULL_FACE);

	// On centre la texture sur le point
	glTexEnvf(GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_TRUE);
	// Défini le mode de transparence, utilisation de la couche alpha
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	for(unsigned int i = 0; i < CTRL_GENERATEUR_PARTICULE_NOMBRE; i++)
	{
		const CParticleSystem *pGenerateurParticule = pControleurParticule->GetGenerateurParIndex (i);
		if (!pGenerateurParticule)
			continue;

		// On sélectionne la texture du générateur de particules
		pGenerateurParticule->GetTexture ()->bind();
		// On indique la taille des particules
		glPointSize(pGenerateurParticule->GetTaille());
		// On affiche les particules sous formes de points
		glBegin(GL_POINTS);
		for(unsigned int j = 0; j < PARTICULE_NOMBRE; j++)
		{
			const CParticule *pParticule = pGenerateurParticule->GetParticuleParIndex(j);
			if(!pParticule)
				continue;
			glm::vec3 Couleur = pGenerateurParticule->GetCouleur ();
			glColor4f (Couleur.r, Couleur.g, Couleur.b, pParticule->GetTransparence());
			glm::vec2 Position = pParticule->GetPosition ();
			glVertex2fv(&Position[0]);
		}
		glEnd();
	}

	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);
	glDisable(GL_POINT_SPRITE_ARB);
	glDisable(GL_TEXTURE_2D);
}

// Les particules sont affichés sous forme de quadrangles si l'extenion GL_ARB_point_sprtie n'est pas supporté
void CAffichage::ParticuleQuad() const
{
	CParticleManager const * pControleurParticule = CParticleManager::Instance();

	// Activation des textures
	glEnable(GL_TEXTURE_2D);
	// Activation de la tranparence
	glEnable(GL_BLEND);
	// Activation du masquage des faces arrières
	glEnable(GL_CULL_FACE);

	// Défini le mode de transparence, le canal alpha est utilisé pour définir la transparence.
	// Si Alpha = 1 alors la couleur RGB est totalement opaque.
	// Si Alpha = 0 alors la couleur de RGB est totalement transparente.
	// Sinon la couleur RGB est plus ou moins opaque.
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	for(unsigned int i = 0; i < CTRL_GENERATEUR_PARTICULE_NOMBRE; i++)
	{
		CParticleSystem const * pGenerateurParticule = pControleurParticule->GetGenerateurParIndex (i);
		if(!pGenerateurParticule)
			continue;

		// On sélectionne la texture du générateur de particules
		pGenerateurParticule->GetTexture()->bind ();
		float fTaille = pGenerateurParticule->GetTaille () / 2;
		// On affiche les particules sous formes de quadrangles
		glBegin (GL_QUADS);
		for(unsigned int j = 0; j < PARTICULE_NOMBRE; j++)
		{
			CParticule const * pParticule = pGenerateurParticule->GetParticuleParIndex(j);
			if(!pParticule)
				continue;
			glm::vec3 Couleur = pGenerateurParticule->GetCouleur ();
			glm::vec2 Position = pParticule->GetPosition ();
			// On indique la couleur de la particule avec la valeur de alpha comme étant la transparence de la particule
			glColor4f(Couleur.r, Couleur.g, Couleur.b, pParticule->GetTransparence());
			glTexCoord2f(0, 0); glVertex2fv(&(Position + glm::vec2(-fTaille, -fTaille))[0]);
			glTexCoord2f(1, 0); glVertex2fv(&(Position + glm::vec2(fTaille, -fTaille))[0]);
			glTexCoord2f(1, 1); glVertex2fv(&(Position + glm::vec2(fTaille, fTaille))[0]);
			glTexCoord2f(0, 1); glVertex2fv(&(Position + glm::vec2(-fTaille, fTaille))[0]);
		}
		glEnd();
	}

	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
}

/*
Menu
*/

// Affichage du menu de la page principale
void CAffichage::MenuPrincipal(unsigned int uiCurseur, bool bJeuCree)
{
	int x = FENETRE_LARGEUR / 2 - 128;
	int y = FENETRE_HAUTEUR / 2;
	m_Font.SetCouleur(glm::vec3(1.0f));
	if (bJeuCree)
		m_Font.Print (x, y + 16 * 4, MENU_TEXTE_REPRENDRE.c_str());
	m_Font.Print (x, y + 16 * 3, MENU_TEXTE_COMMENCER.c_str());
	m_Font.Print (x, y + 16 * 2, MENU_TEXTE_SCORES.c_str());
	m_Font.Print (x, y + 16 * 1, MENU_TEXTE_QUITTER.c_str());
	m_Font.Print (x - 32, y + 16 * uiCurseur, ">");
}

// Affichage du menu de la page commencer
void CAffichage::MenuCommencer(unsigned int uiCurseur, unsigned int m_uiNombreJoueur, const std::string & szNom1, const std::string & szNom2)
{
	int x = FENETRE_LARGEUR / 2 - 128;
	int y = FENETRE_HAUTEUR / 2;

	m_Font.SetCouleur(glm::vec3(1.0f));
	m_Font.Print(FONT_POSITION_CENTRE, FENETRE_HAUTEUR / 2 + 32, "COMMENCER PARTIE");
	m_Font.Print(x, y + 16 * 3 - 48, "Nombre de joueurs : %d", m_uiNombreJoueur);
	m_Font.Print(x, y + 16 * 2 - 48, "Joueur 1 : %s", szNom1.c_str ());
	if(m_uiNombreJoueur == 2) 
		m_Font.Print(x, y + 16 * 1 - 48, "Joueur 2 : %s", szNom2.c_str ());
	m_Font.Print(FONT_POSITION_CENTRE, y + 16 * 0 - 48, "Commencer");
	m_Font.Print(x - 32, y + 16 * (uiCurseur - 1) - 48, ">");
}

// Affichage des scores
void CAffichage::MenuScore()
{
	// Chargement des scores. C'est le rôle de notre classe CXMLScore.
	CXMLScore XMLScore;
	XMLScore.Charger((DIRECTORY + FICHIER_XML_SCORE).c_str());
	std::multiset<SScore, std::greater<SScore> > const & ListeScores = XMLScore.GetListeScores ();

	m_Font.SetCouleur(glm::vec3(1.0f));
	m_Font.Print (FONT_POSITION_CENTRE, FENETRE_HAUTEUR / 2 + 32, "HIGH SCORES");

	// Parcours de la liste des scores, à partir du meilleur et au maximum 10.
	std::multiset<SScore, std::greater<SScore> >::const_iterator it_set;
	int i = 0;
	for(it_set = ListeScores.begin (); it_set != ListeScores.end () && i < 10; it_set++, i++)
	{
		unsigned int uiScoreTotal = 0;
		// La classe ostringstream permet la création de chaîne formater, pour la conversion de nombres en chaines ici.
		std::ostringstream m_szLigne;
		for (std::list<SJoueur>::const_iterator it_list = it_set->m_Joueur.begin (); it_list != it_set->m_Joueur.end (); it_list++)
		{
			m_szLigne << it_list->m_szNom;
			m_szLigne << ":";
			m_szLigne << it_list->m_uiScore;
			m_szLigne << " ";
			uiScoreTotal += it_list->m_uiScore;
		}
		m_Font.SetCouleur(glm::vec3(1.f));
		m_Font.Print (FENETRE_LARGEUR * 3 / 8, FENETRE_HAUTEUR / 2 - 16 * i, m_szLigne.str ().c_str ());
		
		std::ostringstream m_szTotal;
		m_szTotal << uiScoreTotal;
		m_Font.SetCouleur(glm::vec3(0.f, 1.f, 0.f));
		m_Font.Print (FENETRE_LARGEUR * 2 / 8, FENETRE_HAUTEUR / 2 - 16 * i, m_szTotal.str ().c_str ());
	}
}

// Affichage de la page chargement
void CAffichage::MenuChargement ()
{
	m_Font.Print (FONT_POSITION_CENTRE, FENETRE_HAUTEUR / 2 + 32, "CHARGEMENT ...");
	m_iFpsNiveau = 0;
}

// Affichage d'un message d'erreurs s'il y a un problème pendant le chargement du fichier XML.
void CAffichage::MenuChargementErreur ()
{
	m_Font.Print (FONT_POSITION_CENTRE, FENETRE_HAUTEUR / 2 + 32, "ERREUR");
	m_Font.Print (FONT_POSITION_CENTRE, FENETRE_HAUTEUR / 2 + 0, "Le fichier 'niveau.xml' n'est pas valide");
	m_Font.Print (FONT_POSITION_CENTRE, FENETRE_HAUTEUR / 2 - 16, "Consulter le fichier 'stderr.txt' pour plus d'informations");
}

// Affichage d'une image de fond pour les menus
void CAffichage::MenuImageFond(texture * pTexture) const
{
	glEnable(GL_TEXTURE_2D);

	pTexture->bind();
	glBegin(GL_QUADS);
		glColor3f(1.0f, 1.0f, 1.0f);
		glTexCoord2f(0, 0); glVertex2f(0, 0);
		glTexCoord2f(1, 0); glVertex2f(float(FENETRE_LARGEUR), 0);
		glTexCoord2f(1, 1); glVertex2f(float(FENETRE_LARGEUR), float(FENETRE_HAUTEUR));
		glTexCoord2f(0, 1); glVertex2f(0, float(FENETRE_HAUTEUR));
	glEnd();
}

/*
HUD
*/

// Compte et affiche le nombre d'images par seconde du jeu
void CAffichage::Fps()
{
	if (m_iFpsNiveau == 0)
		m_JeuTimer.Init ();
	
	m_iFpsNiveau++;
	m_iFpsCompte++;
	if (m_FpsTimer.GetTime () > 1.0f)
	{
		m_FpsTimer.Init ();
		m_iFps = m_iFpsCompte;
		m_iFpsCompte = 0;
	}

	m_Font.SetCouleur(glm::vec3(1.0f));
	m_Font.Print (0, 16, "%d FPS", m_iFps);
	m_Font.Print (0, 0, "Moyenne : %4.0f FPS", m_iFpsNiveau / m_JeuTimer.GetTime ());
}

// Affichage de l'échec d'une partie pour deux joueurs
void CAffichage::HUDDefaite (unsigned int uiJoueur1, const char* szJoueur1, unsigned int uiJoueur2, const char* szJoueur2)
{
	Fps ();

	m_Font.SetCouleur(glm::vec3(1.0f));
	m_Font.Print (FONT_POSITION_CENTRE, FENETRE_HAUTEUR / 2 + 32, "DEFAITE ...");
	m_Font.Print (FONT_POSITION_CENTRE, FENETRE_HAUTEUR / 2 - 0, "Les forces de l'ignoble Zglu vous ont vaincu");
	m_Font.Print (FONT_POSITION_CENTRE, FENETRE_HAUTEUR / 2 - 32, "%s : %d - %s : %d", szJoueur1, uiJoueur1, szJoueur2, uiJoueur2);
	m_Font.SetCouleur(glm::vec3(0.0f, 1.0f, 0.0f));
	m_Font.Print (FONT_POSITION_CENTRE, FENETRE_HAUTEUR / 2 - 48, "Total : %d points", uiJoueur1 + uiJoueur2);
	m_Font.SetCouleur(glm::vec3(1.0f, 1.0f, 1.0f));
	m_Font.Print (FONT_POSITION_CENTRE, FENETRE_HAUTEUR / 2 - 96, "Appuyer sur 'echap' pour retourner au menu");	
}

// Affichage de l'échec d'une partie pour un joueur
void CAffichage::HUDDefaite (unsigned int uiJoueur1, const char* szJoueur1)
{
	Fps ();

	m_Font.SetCouleur(glm::vec3(1.0f));
	m_Font.Print (FONT_POSITION_CENTRE, FENETRE_HAUTEUR / 2 + 32, "DEFAITE ...");
	m_Font.Print (FONT_POSITION_CENTRE, FENETRE_HAUTEUR / 2 - 0, "Les forces de l'ignoble Zglu vous ont vaincu");
	m_Font.SetCouleur(glm::vec3(0.0f, 1.0f, 0.0f));
	m_Font.Print (FONT_POSITION_CENTRE, FENETRE_HAUTEUR / 2 - 48, "%s : %d points", szJoueur1, uiJoueur1);
	m_Font.SetCouleur(glm::vec3(1.0f, 1.0f, 1.0f));
	m_Font.Print (FONT_POSITION_CENTRE, FENETRE_HAUTEUR / 2 - 96, "Appuyer sur 'echap' pour retourner au menu");	
}

// Affichage de la victoire d'une partie pour deux joueurs
void CAffichage::HUDVictoire (unsigned int uiJoueur1, const char* szJoueur1, unsigned int uiJoueur2, const char* szJoueur2)
{
	Fps ();

	m_Font.SetCouleur(glm::vec3(1.0f));
	m_Font.Print (FONT_POSITION_CENTRE, FENETRE_HAUTEUR / 2 + 32, "VICTOIRE !");
	m_Font.Print (FONT_POSITION_CENTRE, FENETRE_HAUTEUR / 2 - 0, "Vous avez vaincu les forces de l'ignoble Zglu");
	m_Font.Print (FONT_POSITION_CENTRE, FENETRE_HAUTEUR / 2 - 32, "%s : %d - %s : %d", szJoueur1, uiJoueur1, szJoueur2, uiJoueur2);
	m_Font.SetCouleur(glm::vec3(0.0f, 1.0f, 0.0f));
	m_Font.Print (FONT_POSITION_CENTRE, FENETRE_HAUTEUR / 2 - 48, "Total : %d points", uiJoueur1 + uiJoueur2);
	m_Font.SetCouleur(glm::vec3(1.0f, 1.0f, 1.0f));
	m_Font.Print (FONT_POSITION_CENTRE, FENETRE_HAUTEUR / 2 - 96, "Appuyer sur 'echap' pour retourner au menu");	
}

// Affichage de la victoire d'une partie pour un joueur
void CAffichage::HUDVictoire (unsigned int uiJoueur1, const char* szJoueur1)
{
	Fps ();

	m_Font.SetCouleur(glm::vec3(1.0f));
	m_Font.Print (FONT_POSITION_CENTRE, FENETRE_HAUTEUR / 2 + 32, "VICTOIRE !");
	m_Font.Print (FONT_POSITION_CENTRE, FENETRE_HAUTEUR / 2 - 0, "Vous avez vaincu les forces de l'ignoble Zglu");
	m_Font.SetCouleur(glm::vec3(0.0f, 1.0f, 0.0f));
	m_Font.Print (FONT_POSITION_CENTRE, FENETRE_HAUTEUR / 2 - 48, "%s : %d points", szJoueur1, uiJoueur1);
	m_Font.SetCouleur(glm::vec3(1.0f, 1.0f, 1.0f));
	m_Font.Print (FONT_POSITION_CENTRE, FENETRE_HAUTEUR / 2 - 96, "Appuyer sur 'echap' pour retourner au menu");	
}

// Affichage d'insformations utiles pendant le jeu pour une partie à deux joueurs : Noms, scores, FPS, numéro du niveau.
void CAffichage::HUDJeu (unsigned int uiNiveau, unsigned int uiJoueur1, const char* szJoueur1, unsigned int uiJoueur2, const char* szJoueur2)
{
	int iChiffres = 2;
	int iScore = uiJoueur2;
	while (iScore /= 10)
		iChiffres++;

	Fps ();

	m_Font.SetCouleur(glm::vec3(0.0f, 1.0f, 0.0f));
	m_Font.Print (FONT_POSITION_CENTRE_HAUT, "Niveau %d - Scores : %d", uiNiveau + 1, uiJoueur1 + uiJoueur2);
	m_Font.SetCouleur(glm::vec3(0.5f, 0.5f, 1.0f));
	m_Font.Print (0, FENETRE_HAUTEUR - 16, "%s", szJoueur1);
	m_Font.Print (0, FENETRE_HAUTEUR - 32, "%d", uiJoueur1);
	m_Font.SetCouleur(glm::vec3(1.0f, 0.5f, 0.5f));
	m_Font.Print (FENETRE_LARGEUR - 12 * (strlen (szJoueur2) + 1), FENETRE_HAUTEUR - 16, "%s", szJoueur2);
	m_Font.Print (FENETRE_LARGEUR - 12 * iChiffres, FENETRE_HAUTEUR - 32, "%d", uiJoueur2);
}

// Affichage d'insformations utiles pendant le jeu pour une partie à un seul joueur : Nom, score, FPS, numéro du niveau.
void CAffichage::HUDJeu (unsigned int uiNiveau, unsigned int uiJoueur1, const char* szJoueur1)
{
	Fps ();

	m_Font.SetCouleur(glm::vec3(0.0f, 1.0f, 0.0f));
	m_Font.Print (FONT_POSITION_CENTRE_HAUT, "Niveau %d", uiNiveau + 1);
	m_Font.SetCouleur(glm::vec3(0.5f, 0.5f, 1.0f));
	m_Font.Print (0, FENETRE_HAUTEUR - 16, "%s", szJoueur1);
	m_Font.Print (0, FENETRE_HAUTEUR - 32, "%d", uiJoueur1);
}

// Affichage du HUD lors de la fin d'un niveau pour une partie à deux joueurs.
void CAffichage::HUDFinNiveau (unsigned int uiNiveau, unsigned int uiJoueur1, const char* szJoueur1, unsigned int uiJoueur2, const char* szJoueur2)
{
	int iChiffres = 2;
	int iScore = uiJoueur2;
	while (iScore /= 10)
		iChiffres++;

	Fps ();

	m_Font.SetCouleur(glm::vec3(0.0f, 1.0f, 0.0f));
	m_Font.Print (FONT_POSITION_CENTRE_HAUT, "Niveau %d - Scores : %d", uiNiveau + 1, uiJoueur1 + uiJoueur2);
	m_Font.SetCouleur(glm::vec3(0.5f, 0.5f, 1.0f));
	m_Font.Print (0, FENETRE_HAUTEUR - 16, "%s", szJoueur1);
	m_Font.Print (0, FENETRE_HAUTEUR - 32, "%d", uiJoueur1);
	m_Font.SetCouleur(glm::vec3(1.0f, 0.5f, 0.5f));
	m_Font.Print (FENETRE_LARGEUR - 12 * (strlen (szJoueur2) + 1), FENETRE_HAUTEUR - 16, "%s", szJoueur2);
	m_Font.Print (FENETRE_LARGEUR - 12 * iChiffres, FENETRE_HAUTEUR - 32, "%d", uiJoueur2);

	m_Font.SetCouleur(glm::vec3(0.0f, 1.0f, 0.0f));
	m_Font.Print (FONT_POSITION_CENTRE, FENETRE_HAUTEUR / 2, "Appuyez sur 'entree' pour passer au niveau %d", uiNiveau + 2);
}

// Affichage du HUD lors de la fin d'un niveau pour une partie à un joueur.
void CAffichage::HUDFinNiveau (unsigned int uiNiveau, unsigned int uiJoueur1, const char* szJoueur1)
{
	Fps ();

	m_Font.SetCouleur(glm::vec3(0.0f, 1.0f, 0.0f));
	m_Font.Print (FONT_POSITION_CENTRE_HAUT, "Niveau %d", uiNiveau + 1);
	m_Font.SetCouleur(glm::vec3(0.5f, 0.5f, 1.0f));
	m_Font.Print (0, FENETRE_HAUTEUR - 16, "%s", szJoueur1);
	m_Font.Print (0, FENETRE_HAUTEUR - 32, "%d", uiJoueur1);

	m_Font.SetCouleur(glm::vec3(0.0f, 1.0f, 0.0f));
	m_Font.Print (FONT_POSITION_CENTRE, FENETRE_HAUTEUR / 2, "Appuyez sur 'entree' pour passer au niveau %d", uiNiveau + 2);
}

/*
Commun
*/

// Initialisation de la zone d'affichage sur l'écran
void CAffichage::Debut() const
{
	// On vide le tampon chromatique
	glClear(GL_COLOR_BUFFER_BIT);
	// On initialise la matrice de visualisation
	glLoadIdentity();

	// On modifie la matrice de projection de telle manière que l'écran soit utilisable en 2D et que l'unité soit le pixel de l'écran.
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity ();
	glOrtho(0, FENETRE_LARGEUR, 0, FENETRE_HAUTEUR, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

// Affiche l'image et inverse les tampons du double buffering
void CAffichage::Fin () const
{
	CWindowSDL::Instance()->Swap ();
}
