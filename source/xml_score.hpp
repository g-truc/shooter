//////////////////////////////////////////////////////////////////////////////
//	Shoot(r)
//////////////////////////////////////////////////////////////////////////////
//! \author Groove (http://www.g-truc.net) / Code(r) (http://www.codermag.com)
//! \date 21/05/2004
//! \file xml_score.h
//! \brief Déclaration de la classe CXMLScore permettant le chargement et l'enregistrement des scores.
//////////////////////////////////////////////////////////////////////////////

#ifndef XML_SCORE_H
#define XML_SCORE_H

#include <libxml/tree.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#include <string>
#include <list>
#include <set>

#define FICHIER_XML_SCORE "score.xml"

//! Score pour un joueur dans une partie.
typedef struct
{
	std::string m_szNom;
	unsigned int m_uiScore;
	unsigned int m_uiNiveau;
} SJoueur;

//! Résultats d'une partie.
typedef struct
{
	std::list<SJoueur> m_Joueur;
} SScore;

inline bool operator < (SScore Score1, SScore Score2);
inline bool operator > (SScore Score1, SScore Score2);

//! Classe de chargement et enregistrement des scores.
class CXMLScore
{
private:
	std::string m_szFilename;
	std::multiset<SScore, std::greater<SScore> > m_Scores;
	xmlDoc* m_pDocument;

	//! Parcours un élément 'scores'
	void Scores (xmlNode* pScores);
	//! Parcours un élément 'score'
	SScore Score (xmlNode* pElementScore);
	//! Parcours un élément joueur
	SJoueur Joueur (xmlNode* pElementJoueur);

public:
	CXMLScore ();
	~CXMLScore ();
	
	//! Chargement des scores
	bool Charger (const char* szFilename);
	//! Enregistrement des scores
	bool Sauver ();
	//! Ajout d'un score
	bool AjouterScore (SScore Score);
	//! Récupère la liste des scores.
	const std::multiset<SScore, std::greater<SScore> > & GetListeScores () const {return m_Scores;}
};

#endif //XML_SCORE_H
