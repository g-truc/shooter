//////////////////////////////////////////////////////////////////////////////
//	Shoot(r)
//////////////////////////////////////////////////////////////////////////////
//! \author Groove (http://www.g-truc.net) / Code(r) (http://www.codermag.com)
//! \date 07/09/2004
//! \file xml_score.cpp
//! \brief Définition de la classe CXMLScore permettant le chargement et l'enregistrement des scores.
//////////////////////////////////////////////////////////////////////////////
// /!\ Reportez vous au magazine pour plus d'informations car pour expliquer le parcours d'un fichier XML il faut un dessin !

#include "xml_score.hpp"
#include <cstdio>

CXMLScore::CXMLScore ()
{

}

CXMLScore::~CXMLScore ()
{
	xmlFreeDoc (m_pDocument);
}

// Chargement du fichier XML contenant les niveaux
bool CXMLScore::Charger(const char* szFilename)
{
	m_szFilename = std::string(szFilename);
	// Parse le document XML et créé l'arbre DOM
	m_pDocument = xmlParseFile(szFilename);

	// Récupère l'élément racine du document
	xmlNode* pScores = xmlDocGetRootElement(m_pDocument);
	if((!xmlStrcmp (pScores->name, (const xmlChar *)"scores")))
		Scores (pScores);

	return true;
}

bool CXMLScore::Sauver ()
{
	// Sauvegarde du fichier XML
	xmlSaveFile (m_szFilename.c_str (), m_pDocument);

	return true;
}

bool CXMLScore::AjouterScore (SScore Score)
{
	xmlNode* pScores = xmlDocGetRootElement (m_pDocument);
	xmlNode* pText;
	
	// Création d'un noeud text
	pText = xmlNewText ((const xmlChar *) "\t");
	// Ajout à l'élément pScores le noeud text
	xmlAddChild (pScores, pText);
	char szBuffer [16];

	// Création d'un élément score
	xmlNode* pScore = xmlNewChild (pScores, 0, (const xmlChar *) "score", 0);

	// Ajout d'un nouvel élément nommé "score"
	for (std::list<SJoueur>::const_iterator JoueurIt = Score.m_Joueur.begin ();
		JoueurIt != Score.m_Joueur.end ();
		JoueurIt++)
	{
		pText = xmlNewText ((const xmlChar *) "\n\t\t");
		xmlAddChild (pScore, pText);

		xmlNode* pJoueur = xmlNewChild (pScore, 0, (const xmlChar *) "joueur", 0);
		xmlNewProp (pJoueur, (const xmlChar *) "nom", (const xmlChar *) JoueurIt->m_szNom.c_str ());
		memset (szBuffer, '\0', 16);
		sprintf (szBuffer, "%d", JoueurIt->m_uiScore);
		xmlNewProp (pJoueur, (const xmlChar *) "score", (const xmlChar *) szBuffer);
		memset (szBuffer, '\0', 16);
		sprintf (szBuffer, "%d", JoueurIt->m_uiNiveau);
		xmlNewProp (pJoueur, (const xmlChar *) "niveau", (const xmlChar *) szBuffer);
	}
	pText = xmlNewText ((const xmlChar *) "\n\t");
	xmlAddChild (pScore, pText);

	pText = xmlNewText ((const xmlChar *) "\n");
	xmlAddChild (pScores, pText);

	return true;
}

// Parcours un élément 'scores'
void CXMLScore::Scores (xmlNode* pScores)
{
	xmlNode* pScore = pScores->children;
	do
	{
		if (pScore->type == XML_ELEMENT_NODE)
			if ((!xmlStrcmp (pScore->name, (const xmlChar *)"score")))
				m_Scores.insert (Score (pScore));
	}
	while ((pScore = pScore->next) != 0);
}

// Parcours un élément 'score'
SScore CXMLScore::Score (xmlNode* pScore)
{
	SScore Score;

	xmlNode* pJoueur = pScore->children;
	if (!pJoueur)
		return Score;

	do
	{
		if (pJoueur->type == XML_ELEMENT_NODE)
			if ((!xmlStrcmp (pJoueur->name, (const xmlChar *)"joueur")))
				Score.m_Joueur.push_back (Joueur (pJoueur));
	}
	while ((pJoueur = pJoueur->next) != 0);

	return Score;
}

// Parcours un élément joueur
SJoueur CXMLScore::Joueur (xmlNode* pJoueur)
{
	SJoueur Joueur;

	xmlElement* pElementJoueur = (xmlElement*) pJoueur;
	xmlAttribute* pAttribute = pElementJoueur->attributes;
	if (!pAttribute)
		return Joueur;

	do
	{
		if (pAttribute->type == XML_ATTRIBUTE_NODE)
		{
			if ((!xmlStrcmp (pAttribute->name, (const xmlChar *)"nom")))
				Joueur.m_szNom = std::string ((const char*) xmlGetProp (pJoueur, pAttribute->name));
			else if ((!xmlStrcmp (pAttribute->name, (const xmlChar *)"score")))
				Joueur.m_uiScore = atoi ((const char*) xmlGetProp (pJoueur, pAttribute->name));
			else if ((!xmlStrcmp (pAttribute->name, (const xmlChar *)"niveau")))
				Joueur.m_uiNiveau = atoi ((const char*) xmlGetProp (pJoueur, pAttribute->name));
		}
	}
	while ((pAttribute = (xmlAttribute*) pAttribute->next) != 0);

	return Joueur;
}

// Redéfinition de l'opérateur < pour comparer deux SScore afin que le conteneur set puisse trier les meilleurs scores
inline bool operator < (SScore Score1, SScore Score2)
{
	unsigned int uiScore1 = 0, uiScore2 = 0;

	for (std::list<SJoueur>::iterator it = Score1.m_Joueur.begin ();
		 it != Score1.m_Joueur.end (); it++)
		uiScore1 += it->m_uiScore;

	for (std::list<SJoueur>::iterator it = Score2.m_Joueur.begin ();
		 it != Score2.m_Joueur.end (); it++)
		uiScore2 += it->m_uiScore;

	return uiScore1 < uiScore2;
}

// Redéfinition de l'opérateur > pour comparer deux SScore afin que le conteneur set puisse trier les meilleurs scores
inline bool operator > (SScore Score1, SScore Score2)
{
	unsigned int uiScore1 = 0, uiScore2 = 0;

	for (std::list<SJoueur>::iterator it = Score1.m_Joueur.begin ();
		 it != Score1.m_Joueur.end (); it++)
		uiScore1 += it->m_uiScore;

	for (std::list<SJoueur>::iterator it = Score2.m_Joueur.begin ();
		 it != Score2.m_Joueur.end (); it++)
		uiScore2 += it->m_uiScore;

	return uiScore1 > uiScore2;
}
