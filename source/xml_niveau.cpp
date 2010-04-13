//////////////////////////////////////////////////////////////////////////////
//	Shoot(r)
//////////////////////////////////////////////////////////////////////////////
//! \author Groove (http://www.g-truc.net) / Code(r) (http://www.codermag.com)
//! \date 21/05/2004
//! \file xml_niveau.cpp
//! \brief Définition de la classe CXMLNiveau permettant la chargement des niveaux
//////////////////////////////////////////////////////////////////////////////
// /!\ Reportez vous au magazine pour plus d'informations car pour expliquer le parcours d'un fichier XML il faut un dessin !

#include "xml_niveau.hpp"
#include "affichage.hpp"
#include "texture_manager.hpp"

CXMLNiveau::CXMLNiveau ()
{

}

// On libère les données sauvées dans les différents dictionnaires.
// En fait, la plus part des données dynamiques sont sauvés ici car elles proviennent du fichier XML
CXMLNiveau::~CXMLNiveau ()
{
	for (std::map<unsigned int, CTypeBonusArme*>::iterator it = m_TypeBonusArme.begin ();
		it != m_TypeBonusArme.end (); it++)
		delete it->second;

	for (std::map<unsigned int, CTypeBonusBouclier*>::iterator it = m_TypeBonusBouclier.begin ();
		it != m_TypeBonusBouclier.end (); it++)
		delete it->second;

	for (std::map<unsigned int, CTypeEnnemi*>::iterator it = m_TypeEnnemi.begin ();
		it != m_TypeEnnemi.end (); it++)
		delete it->second;
}

// Recherche d'un type de bonus d'armes alléatoirement
CTypeBonusArme* CXMLNiveau::GetTypeBonusArme (unsigned int uiNiveau) const
{
	return m_Niveaux[uiNiveau].m_TypeBonusArme[glm::compRand1(std::size_t(0), m_Niveaux[uiNiveau].m_TypeBonusArme.size ())];
}

// Recherche d'un type de bonus de boucliers alléatoirement
CTypeBonusBouclier* CXMLNiveau::GetTypeBonusBouclier (unsigned int uiNiveau) const
{
	return m_Niveaux[uiNiveau].m_TypeBonusBouclier[glm::compRand1(std::size_t(0), m_Niveaux[uiNiveau].m_TypeBonusBouclier.size ())];
}

// Recherche d'un type d'ennemis alléatoirement
CTypeEnnemi* CXMLNiveau::GetTypeEnnemi (unsigned int uiNiveau) const
{
	return m_Niveaux[uiNiveau].m_TypeEnnemi[glm::compRand1(std::size_t(0), m_Niveaux[uiNiveau].m_TypeEnnemi.size ())];
}

// Retourne la durée d'uun niveau.
float CXMLNiveau::GetTemps (unsigned int uiNiveau)
{
	return m_Niveaux[uiNiveau].m_fTemps;
}

// Retourne le nombre d'ennemis dans le niveau uiNiveau
unsigned int CXMLNiveau::GetEnnemiNombre (unsigned int uiNiveau) const
{
	return m_Niveaux[uiNiveau].m_uiEnnemiNombre;
}

// Retourne le nombre de niveau
unsigned int CXMLNiveau::GetNiveauNombre () const
{
	return m_Niveaux.size ();
}

// Retourne la fréquence de création d'entités de type bonus d'armes
float CXMLNiveau::GetBonusArmeFrequence (unsigned int uiNiveau) const
{
	return m_Niveaux[uiNiveau].m_fBonusArmeFrequence;
}

// Retourne la fréquence de création d'entités de type bonus de boucliers
float CXMLNiveau::GetBonusBouclierFrequence (unsigned int uiNiveau) const
{
	return m_Niveaux[uiNiveau].m_fBonusBouclierFrequence;
}

// Retourne la fréquence de création d'entités de type ennemis
float CXMLNiveau::GetEnnemiFrequence (unsigned int uiNiveau) const
{
	return m_Niveaux[uiNiveau].m_fEnnemiFrequence;
}

// Chargement du fichier XML contenant les niveaux
bool CXMLNiveau::Charger(const char* szFilename)
{
	// Parse le document XML et créé l'arbre DOM
	xmlDoc* pDocument = xmlParseFile(szFilename);

	// Validation du document XML avec sa DTD
	if (!Validation (pDocument))
	{
		xmlFreeDoc (pDocument);
		return false;
	}

	// Récupère l'élément racine du document
	xmlNode* pJeu = xmlDocGetRootElement (pDocument);
	if ((!xmlStrcmp (pJeu->name, (const xmlChar *)"jeu")))
		Jeu (pJeu);

	// Libère la mémoire réservé pour l'arbre DOM
	xmlFreeDoc (pDocument);

	return true;
}

// Validation du document XML avec sa DTD
bool CXMLNiveau::Validation (xmlDoc* pDocument)
{
	// Création d'un contexte de validation pour la sortie des erreurs
	xmlValidCtxt Contexte;
	Contexte.userData = (void *) stderr;
	Contexte.error    = (xmlValidityErrorFunc) fprintf;
	Contexte.warning  = (xmlValidityWarningFunc) fprintf;

	return xmlValidateDocument (&Contexte, pDocument) == 1;
}

// Parcours un élément 'jeu'
void CXMLNiveau::Jeu (xmlNode* pNode)
{
	// Parcours des éléments fils
	xmlNode* pChild = pNode->children;
	if (pChild)
	{
		do
		{
			if (pChild->type == XML_ELEMENT_NODE)
			{
				if ((!xmlStrcmp (pChild->name, (const xmlChar *)"bonus-armes")))
					BonusArmes (pChild);
				else if ((!xmlStrcmp (pChild->name, (const xmlChar *)"bonus-boucliers")))
					BonusBoucliers (pChild);
				else if ((!xmlStrcmp (pChild->name, (const xmlChar *)"ennemis")))
					Ennemis (pChild);
				else if ((!xmlStrcmp (pChild->name, (const xmlChar *)"niveaux")))
					Niveaux (pChild);
			}
		}
		while ((pChild = pChild->next) != 0);
	}
}

// Parcours un élement 'bonus-armes'
void CXMLNiveau::BonusArmes (xmlNode* pNode)
{
	// Parcours des éléments fils
	xmlNode* pChild = pNode->children;
	do
	{
		if (pChild->type == XML_ELEMENT_NODE)
			if ((!xmlStrcmp (pChild->name, (const xmlChar *)"bonus-arme")))
				m_TypeBonusArme.insert (BonusArme (pChild));
	}
	while ((pChild = pChild->next) != 0);
}

// Parcours un élément 'bonus-arme'
std::pair<unsigned int, CTypeBonusArme*> CXMLNiveau::BonusArme (xmlNode* pNode)
{
	CTypeBonusArme *pTypeBonusArme = new CTypeBonusArme;
	unsigned int uiIndex = 0;
	glm::vec2 Vitesse, Acceleration;

	// Parcours des attributs
	xmlAttribute* pAttribute = ((xmlElement*)pNode)->attributes;
	if (pAttribute)
	{
		do
		{
			if (pAttribute->type == XML_ATTRIBUTE_NODE)
			{
				if ((!xmlStrcmp (pAttribute->name, (const xmlChar *)"index")))
					uiIndex = atoi ((const char*) xmlGetProp (pNode, pAttribute->name));
				else if ((!xmlStrcmp (pAttribute->name, (const xmlChar *)"taille")))
					pTypeBonusArme->SetTaille ((float) atof ((const char*) xmlGetProp (pNode, pAttribute->name)));
				else if ((!xmlStrcmp (pAttribute->name, (const xmlChar *)"acceleration-x")))
					Acceleration.x = (float) atof ((const char*) xmlGetProp (pNode, pAttribute->name));
				else if ((!xmlStrcmp (pAttribute->name, (const xmlChar *)"acceleration-y")))
					Acceleration.y = (float) atof ((const char*) xmlGetProp (pNode, pAttribute->name));
				else if ((!xmlStrcmp (pAttribute->name, (const xmlChar *)"vitesse-x")))
					Vitesse.x = (float) atof ((const char*) xmlGetProp (pNode, pAttribute->name));
				else if ((!xmlStrcmp (pAttribute->name, (const xmlChar *)"vitesse-y")))
					Vitesse.y = (float) atof ((const char*) xmlGetProp (pNode, pAttribute->name));
				else if ((!xmlStrcmp (pAttribute->name, (const xmlChar *)"texture")))
					pTypeBonusArme->SetTexture (textureManager::instance()->createTexture2D ((const char*) xmlGetProp (pNode, pAttribute->name)));
				else if ((!xmlStrcmp (pAttribute->name, (const xmlChar *)"son")))
					pTypeBonusArme->SetSon ((const char*) xmlGetProp (pNode, pAttribute->name));
			}
		}
		while ((pAttribute = (xmlAttribute*) pAttribute->next) != 0);
	}

	pTypeBonusArme->SetAcceleration(Acceleration);
	pTypeBonusArme->SetVitesse(Vitesse);

	// Parcours des éléments fils
	xmlNode* pChild = pNode->children;
	if (pChild)
	{
		do
		{
			if (pChild->type == XML_ELEMENT_NODE)
				if ((!xmlStrcmp (pChild->name, (const xmlChar *)"arme")))
					pTypeBonusArme->SetArme (Arme (pChild));
		}
		while ((pChild = pChild->next) != 0);
	}

	return std::make_pair<unsigned int, CTypeBonusArme*> (uiIndex, pTypeBonusArme);
}

// Parcours un élément 'ancrage'
glm::vec2 CXMLNiveau::Ancrage (xmlNode* pNode)
{
	glm::vec2 Ancrage;

	// Parcours des attributs
	xmlAttribute* pAttribute = ((xmlElement*) pNode)->attributes;
	if (pAttribute)
	{
		do
		{
			if (pAttribute->type == XML_ATTRIBUTE_NODE)
			{
				if ((!xmlStrcmp (pAttribute->name, (const xmlChar *)"x")))
					Ancrage.x = (float) atof ((const char*) xmlGetProp (pNode, pAttribute->name));
				else if ((!xmlStrcmp (pAttribute->name, (const xmlChar *)"y")))
					Ancrage.y = (float) atof ((const char*) xmlGetProp (pNode, pAttribute->name));
			}
		}
		while ((pAttribute = (xmlAttribute*) pAttribute->next) != 0);
	}
	return Ancrage;
}

// Parcours un élément 'bonus-boucliers'
void CXMLNiveau::BonusBoucliers (xmlNode* pNode)
{
	// Parcours des éléments
	xmlNode* pChild = pNode->children;
	if (pChild)
	{
		do
		{
			if (pChild->type == XML_ELEMENT_NODE)
				if ((!xmlStrcmp (pChild->name, (const xmlChar *)"bonus-bouclier")))
					m_TypeBonusBouclier.insert (BonusBouclier (pChild));
		}
		while ((pChild = pChild->next) != 0);
	}
}

// Parcours un élément 'bonus-bouclier'
std::pair<unsigned int, CTypeBonusBouclier*> CXMLNiveau::BonusBouclier (xmlNode* pNode)
{
	CTypeBonusBouclier *pTypeBonusBouclier = new CTypeBonusBouclier;
	unsigned int uiIndex = 0;
	glm::vec2 Vitesse, Acceleration;

	// Parcours des attributs
	xmlAttribute* pAttribute = ((xmlElement*) pNode)->attributes;
	if (pAttribute)
	{
		do
		{
			if (pAttribute->type == XML_ATTRIBUTE_NODE)
			{
				if ((!xmlStrcmp (pAttribute->name, (const xmlChar *)"index")))
					uiIndex = atoi ((const char*) xmlGetProp (pNode, pAttribute->name));
				else if ((!xmlStrcmp (pAttribute->name, (const xmlChar *)"taille")))
					pTypeBonusBouclier->SetTaille ((float) atof ((const char*) xmlGetProp (pNode, pAttribute->name)));
				else if ((!xmlStrcmp (pAttribute->name, (const xmlChar *)"acceleration-x")))
					Acceleration.x = (float) atof ((const char*) xmlGetProp (pNode, pAttribute->name));
				else if ((!xmlStrcmp (pAttribute->name, (const xmlChar *)"acceleration-y")))
					Acceleration.y = (float) atof ((const char*) xmlGetProp (pNode, pAttribute->name));
				else if ((!xmlStrcmp (pAttribute->name, (const xmlChar *)"vitesse-x")))
					Vitesse.x = (float) atof ((const char*) xmlGetProp (pNode, pAttribute->name));
				else if ((!xmlStrcmp (pAttribute->name, (const xmlChar *)"vitesse-y")))
					Vitesse.y = (float) atof ((const char*) xmlGetProp (pNode, pAttribute->name));
				else if ((!xmlStrcmp (pAttribute->name, (const xmlChar *)"texture")))
					pTypeBonusBouclier->SetTexture (textureManager::instance()->createTexture2D ((const char*) xmlGetProp (pNode, pAttribute->name)));
				else if ((!xmlStrcmp (pAttribute->name, (const xmlChar *)"bouclier")))
					pTypeBonusBouclier->SetBonusBouclier (atoi ((const char*) xmlGetProp (pNode, pAttribute->name)));
				else if ((!xmlStrcmp (pAttribute->name, (const xmlChar *)"son")))
					pTypeBonusBouclier->SetSon ((const char*) xmlGetProp (pNode, pAttribute->name));
			}
		}
		while ((pAttribute = (xmlAttribute*) pAttribute->next) != 0);
	}

	pTypeBonusBouclier->SetAcceleration (Acceleration);
	pTypeBonusBouclier->SetVitesse (Vitesse);

	return std::make_pair<unsigned int, CTypeBonusBouclier*> (uiIndex, pTypeBonusBouclier);
}

// Parcours un élément 'ennemis'
void CXMLNiveau::Ennemis (xmlNode* pNode)
{
	// Parcours des éléments
	xmlNode* pChild = pNode->children;
	if (pChild)
	{
		do
		{
			if (pChild->type == XML_ELEMENT_NODE)
				if ((!xmlStrcmp (pChild->name, (const xmlChar *)"ennemi")))
					m_TypeEnnemi.insert (Ennemi (pChild));
		}
		while ((pChild = pChild->next) != 0);
	}
}

// Parcours un élément 'ennemi'
std::pair<unsigned int, CTypeEnnemi*> CXMLNiveau::Ennemi (xmlNode* pNode)
{
	CTypeEnnemi *pTypeEnnemi = new CTypeEnnemi;
	unsigned int uiIndex = 0;
	glm::vec2 Vitesse, Acceleration;

	// Parcours des attributs
	xmlAttribute* pAttribute = ((xmlElement*)pNode)->attributes;
	if (pAttribute)
	{
		do
		{
			if (pAttribute->type == XML_ATTRIBUTE_NODE)
			{
				if ((!xmlStrcmp (pAttribute->name, (const xmlChar *)"index")))
					uiIndex = atoi ((const char*) xmlGetProp (pNode, pAttribute->name));
				else if ((!xmlStrcmp (pAttribute->name, (const xmlChar *)"taille")))
					pTypeEnnemi->SetTaille ((float) atof ((const char*) xmlGetProp (pNode, pAttribute->name)));
				else if ((!xmlStrcmp (pAttribute->name, (const xmlChar *)"dommage")))
					pTypeEnnemi->SetDommage (atoi ((const char*) xmlGetProp (pNode, pAttribute->name)));
				else if ((!xmlStrcmp (pAttribute->name, (const xmlChar *)"bouclier")))
					pTypeEnnemi->SetBouclier (atoi ((const char*) xmlGetProp (pNode, pAttribute->name)));
				else if ((!xmlStrcmp (pAttribute->name, (const xmlChar *)"point")))
					pTypeEnnemi->SetPoint (atoi ((const char*) xmlGetProp (pNode, pAttribute->name)));
				else if ((!xmlStrcmp (pAttribute->name, (const xmlChar *)"acceleration-x")))
					Acceleration.x = (float) atof ((const char*) xmlGetProp (pNode, pAttribute->name));
				else if ((!xmlStrcmp (pAttribute->name, (const xmlChar *)"acceleration-y")))
					Acceleration.y = (float) atof ((const char*) xmlGetProp (pNode, pAttribute->name));
				else if ((!xmlStrcmp (pAttribute->name, (const xmlChar *)"vitesse-x")))
					Vitesse.x = (float) atof ((const char*) xmlGetProp (pNode, pAttribute->name));
				else if ((!xmlStrcmp (pAttribute->name, (const xmlChar *)"vitesse-y")))
					Vitesse.y = (float) atof ((const char*) xmlGetProp (pNode, pAttribute->name));
				else if ((!xmlStrcmp (pAttribute->name, (const xmlChar *)"texture")))
					pTypeEnnemi->SetTexture(textureManager::instance()->createTexture2D ((const char*) xmlGetProp (pNode, pAttribute->name)));
				else if ((!xmlStrcmp (pAttribute->name, (const xmlChar *)"son")))
					pTypeEnnemi->SetSon ((const char*) xmlGetProp (pNode, pAttribute->name));
			}
		}
		while ((pAttribute = (xmlAttribute*) pAttribute->next) != 0);
	}

	pTypeEnnemi->SetAcceleration (Acceleration);
	pTypeEnnemi->SetVitesse (Vitesse);

	// Parcours des éléments
	xmlNode* pChild = pNode->children;
	if (pChild)
	{
		do
		{
			if (pChild->type == XML_ELEMENT_NODE)
				if ((!xmlStrcmp (pChild->name, (const xmlChar*)"arme")))
					pTypeEnnemi->SetTypeArme (Arme (pChild));
		}
		while ((pChild = pChild->next) != 0);
	}

	return std::make_pair<unsigned int, CTypeEnnemi*> (uiIndex, pTypeEnnemi);
}

// Parcours un élément 'arme'
CTypeArme* CXMLNiveau::Arme (xmlNode* pNode)
{
	CTypeArme *pTypeArme = new CTypeArme;

	glm::vec2 Acceleration;
	glm::vec2 Vitesse;
	glm::vec3 Couleur;

	// Parcours des attributs
	xmlAttribute* pAttribute = ((xmlElement*) pNode)->attributes;
	if (pAttribute)
	{
		do
		{
			if (pAttribute->type == XML_ATTRIBUTE_NODE)
			{
				if ((!xmlStrcmp (pAttribute->name, (const xmlChar *)"dommage")))
					pTypeArme->SetDommage (atoi ((const char*) xmlGetProp (pNode, pAttribute->name)));
				else if ((!xmlStrcmp (pAttribute->name, (const xmlChar *)"cadence")))
					pTypeArme->SetCadence ((float) atof ((const char*) xmlGetProp (pNode, pAttribute->name)));
				else if ((!xmlStrcmp (pAttribute->name, (const xmlChar *)"taille")))
					pTypeArme->SetTaille ((float) atof ((const char*) xmlGetProp (pNode, pAttribute->name)));
				else if ((!xmlStrcmp (pAttribute->name, (const xmlChar *)"precision")))
					pTypeArme->SetPrecision ((float) atof ((const char*) xmlGetProp (pNode, pAttribute->name)));
				else if ((!xmlStrcmp (pAttribute->name, (const xmlChar *)"acceleration-x")))
					Acceleration.x = (float) atof ((const char*) xmlGetProp (pNode, pAttribute->name));
				else if ((!xmlStrcmp (pAttribute->name, (const xmlChar *)"acceleration-y")))
					Acceleration.y = (float) atof ((const char*) xmlGetProp (pNode, pAttribute->name));
				else if ((!xmlStrcmp (pAttribute->name, (const xmlChar *)"vitesse-x")))
					Vitesse.x = (float) atof ((const char*) xmlGetProp (pNode, pAttribute->name));
				else if ((!xmlStrcmp (pAttribute->name, (const xmlChar *)"vitesse-y")))
					Vitesse.y = (float) atof ((const char*) xmlGetProp (pNode, pAttribute->name));
				else if ((!xmlStrcmp (pAttribute->name, (const xmlChar *)"texture")))
					pTypeArme->SetTexture(textureManager::instance()->createTexture2D ((const char*) xmlGetProp (pNode, pAttribute->name)));
				else if ((!xmlStrcmp (pAttribute->name, (const xmlChar *)"son")))
					pTypeArme->SetSon ((const char*) xmlGetProp (pNode, pAttribute->name));
				else if ((!xmlStrcmp (pAttribute->name, (const xmlChar *)"fumee")))
					pTypeArme->SetFumee (!xmlStrcmp (xmlGetProp (pNode, pAttribute->name), (const xmlChar *)"oui"));
				else if ((!xmlStrcmp (pAttribute->name, (const xmlChar *)"rouge")))
					Couleur.r = (float) atof ((const char*) xmlGetProp (pNode, pAttribute->name));
				else if ((!xmlStrcmp (pAttribute->name, (const xmlChar *)"vert")))
					Couleur.g = (float) atof ((const char*) xmlGetProp (pNode, pAttribute->name));
				else if ((!xmlStrcmp (pAttribute->name, (const xmlChar *)"bleu")))
					Couleur.b = (float) atof ((const char*) xmlGetProp (pNode, pAttribute->name));
			}
		}
		while ((pAttribute = (xmlAttribute*) pAttribute->next) != 0);
	}

	pTypeArme->SetAcceleration (Acceleration);
	pTypeArme->SetVitesse (Vitesse);
	pTypeArme->SetCouleur (Couleur);

	// Parcours des éléments
	xmlNode* pChild = pNode->children;
	if (pChild)
	{
		do
		{
			if (pChild->type == XML_ELEMENT_NODE)
				if ((!xmlStrcmp (pChild->name, (const xmlChar *)"ancrage")))
					pTypeArme->AddAncrage (Ancrage (pChild));
		}
		while ((pChild = pChild->next) != 0);
	}

	return pTypeArme;
}

// Parcours un élément 'niveaux'
void CXMLNiveau::Niveaux (xmlNode* pNode)
{
	// Parcours des éléments
	xmlNode* pChild = pNode->children;
	if (pChild)
	{
		do
		{
			if (pChild->type == XML_ELEMENT_NODE)
				if (!xmlStrcmp (pChild->name, (const xmlChar *)"niveau"))
					m_Niveaux.push_back (Niveau (pChild));
		}
		while ((pChild = pChild->next) != 0);
	}
}

// Parcours un élément 'niveau'
SNiveau CXMLNiveau::Niveau (xmlNode* pNode)
{
	SNiveau Niveau;

	// Parcours des attributs
	xmlAttribute* pAttribute = ((xmlElement*) pNode)->attributes;
	if (pAttribute)
	{
		do
		{
			if (pAttribute->type == XML_ATTRIBUTE_NODE)
			{
				if ((!xmlStrcmp (pAttribute->name, (const xmlChar *)"temps")))
					Niveau.m_fTemps = (float) atof ((const char*) xmlGetProp (pNode, pAttribute->name));
			}
		}
		while ((pAttribute = (xmlAttribute*) pAttribute->next) != 0);
	}

	// Parcours des éléments fils
	xmlNode* pChild = pNode->children;
	if (pChild)
	{
		do
		{
			if (pChild->type == XML_ELEMENT_NODE)
			{
				if ((!xmlStrcmp (pChild->name, (const xmlChar *)"index-armes")))
					IndexArmes (pChild, Niveau);
				else if ((!xmlStrcmp (pChild->name, (const xmlChar *)"index-boucliers")))
					IndexBoucliers (pChild, Niveau);
				else if ((!xmlStrcmp (pChild->name, (const xmlChar *)"index-ennemis")))
					IndexEnnemis (pChild, Niveau);
			}
		}
		while ((pChild = pChild->next) != 0);
	}

	return Niveau;
}

// Parcours un élément 'index-armes'
void CXMLNiveau::IndexArmes (xmlNode* pNode, SNiveau & Niveau)
{
	// Parcours des attributs
	xmlAttribute* pAttribute = ((xmlElement*) pNode)->attributes;
	if (pAttribute)
	{
		do
		{
			if (pAttribute->type == XML_ATTRIBUTE_NODE)
				if ((!xmlStrcmp (pAttribute->name, (const xmlChar *)"frequence")))
					Niveau.m_fBonusArmeFrequence = (float) atof ((const char*) xmlGetProp (pNode, pAttribute->name));
		}
		while ((pAttribute = (xmlAttribute*) pAttribute->next) != 0);
	}

	// Parcours des éléments fils
	xmlNode* pChild = pNode->children;
	if (pChild)
	{
		do
		{
			if (pChild->type == XML_ELEMENT_NODE)
				if ((!xmlStrcmp (pChild->name, (const xmlChar *)"index-arme")))
					Niveau.m_TypeBonusArme.push_back (GetTypeBonusArmeParIndex (IndexAttribute (pChild)));
		}
		while ((pChild = pChild->next) != 0);
	}
}

// Parcours un élément 'index-boucliers'
void CXMLNiveau::IndexBoucliers (xmlNode* pNode, SNiveau & Niveau)
{
	// Parcours des attributs
	xmlAttribute* pAttribute = ((xmlElement*) pNode)->attributes;
	if (pAttribute)
	{
		do
		{
			if (pAttribute->type == XML_ATTRIBUTE_NODE)
				if ((!xmlStrcmp (pAttribute->name, (const xmlChar *)"frequence")))
					Niveau.m_fBonusBouclierFrequence = (float) atof ((const char*) xmlGetProp (pNode, pAttribute->name));
		}
		while ((pAttribute = (xmlAttribute*) pAttribute->next) != 0);
	}

	// Parcours des éléments fils
	xmlNode* pChild = pNode->children;
	if (pChild)
	{
		do
		{
			if (pChild->type == XML_ELEMENT_NODE)
				if ((!xmlStrcmp (pChild->name, (const xmlChar *)"index-bouclier")))
					Niveau.m_TypeBonusBouclier.push_back (GetTypeBonusBouclierParIndex (IndexAttribute (pChild)));
		}
		while ((pChild = pChild->next) != 0);
	}
}

// Parcours un élément 'index-ennemis'
void CXMLNiveau::IndexEnnemis (xmlNode* pNode, SNiveau & Niveau)
{
	// Parcours des attributs
	xmlAttribute* pAttribute = ((xmlElement*) pNode)->attributes;
	if (pAttribute)
	{
		do
		{
			if (pAttribute->type == XML_ATTRIBUTE_NODE)
			{
				if ((!xmlStrcmp (pAttribute->name, (const xmlChar *)"nombre")))
					Niveau.m_uiEnnemiNombre = atoi ((const char*) xmlGetProp (pNode, pAttribute->name));
				else if ((!xmlStrcmp (pAttribute->name, (const xmlChar *)"frequence")))
					Niveau.m_fEnnemiFrequence = (float) atof ((const char*) xmlGetProp (pNode, pAttribute->name));
			}
		}
		while ((pAttribute = (xmlAttribute*) pAttribute->next) != 0);
	}

	// Parcours des éléments fils
	xmlNode* pChild = pNode->children;
	if (pChild)
	{
		do
		{
			if (pChild->type == XML_ELEMENT_NODE)
				if ((!xmlStrcmp (pChild->name, (const xmlChar *)"index-ennemi")))
					Niveau.m_TypeEnnemi.push_back (GetTypeEnnemiParIndex (IndexAttribute (pChild)));
		}
		while ((pChild = pChild->next) != 0);
	}
}

// Parcours les attributs d'un élément 'index-*'
int CXMLNiveau::IndexAttribute (xmlNode* pNode)
{
	// Parcours des attributs
	xmlAttribute* pAttribute = ((xmlElement*) pNode)->attributes;
	if (pAttribute)
	{
		do
		{
			if (pAttribute->type == XML_ATTRIBUTE_NODE)
				if ((!xmlStrcmp (pAttribute->name, (const xmlChar *)"index")))
					return atoi ((const char*) xmlGetProp (pNode, pAttribute->name));
		}
		while ((pAttribute = (xmlAttribute*) pAttribute->next) != 0);
	}
	return -1;
}

// Compte le nombre d'enfant d'un élément.
unsigned int CXMLNiveau::CompterChildren (xmlNode* pNode)
{
	unsigned int uiNombreChildren = 0;
	
	// Parcours des éléments fils
	xmlNode* pChild = pNode->children;
	if (pChild)
	{
		do
		{
			if (pChild->type == XML_ELEMENT_NODE)
				uiNombreChildren++;
		}
		while ((pChild = pChild->next) != 0);
	}
	return uiNombreChildren;
}

// Recherche d'un bonus d'armes dans le dictionnaire d'armes
CTypeBonusArme* CXMLNiveau::GetTypeBonusArmeParIndex (unsigned int uiIndex) const
{
	return m_TypeBonusArme.find (uiIndex)->second;
}

// Recherche d'un bonus de boucliers dans le dictionnaire de boucliers
CTypeBonusBouclier* CXMLNiveau::GetTypeBonusBouclierParIndex (unsigned int uiIndex) const
{
	return m_TypeBonusBouclier.find (uiIndex)->second;
}

// Recherche d'un ennemi dans le dictionnaire des ennemis
CTypeEnnemi* CXMLNiveau::GetTypeEnnemiParIndex (unsigned int uiIndex) const
{
	return m_TypeEnnemi.find (uiIndex)->second;
}
