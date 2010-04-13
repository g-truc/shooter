//////////////////////////////////////////////////////////////////////////////
//	Shoot(r)
//////////////////////////////////////////////////////////////////////////////
//! \author Groove (http://www.g-truc.net) / Code(r) (http://www.codermag.com)
//! \date 21/05/2004
//! \file xml_niveau.h
//! \brief Déclaration de la classe CXMLNiveau permettant la chargement des niveaux
//////////////////////////////////////////////////////////////////////////////

#ifndef XML_NIVEAU_H
#define XML_NIVEAU_H

#include <libxml/tree.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#include "type_arme.hpp"
#include "type_bonus_arme.hpp"
#include "type_bonus_bouclier.hpp"
#include "type_ennemi.hpp"

#define FICHIER_XML_NIVEAU "niveau.xml"

//! Structure décrivant un niveau : Bonus d'armes, bonus de boucliers et ennemis
typedef struct
{
	float m_fTemps;

	float m_fBonusArmeFrequence;
	std::vector<CTypeBonusArme*> m_TypeBonusArme;

	float m_fBonusBouclierFrequence;
	std::vector<CTypeBonusBouclier*> m_TypeBonusBouclier;

	unsigned int m_uiEnnemiNombre;
	float m_fEnnemiFrequence;
	std::vector<CTypeEnnemi*> m_TypeEnnemi;
} SNiveau;

//! Classe de chargement des niveaux.
class CXMLNiveau
{
private:
	std::vector<SNiveau> m_Niveaux;

	//! Dictionnaire des bonus d'armes
	std::map<unsigned int, CTypeBonusArme*> m_TypeBonusArme;
	//! Dictionnaire des bonus de boucliers
	std::map<unsigned int, CTypeBonusBouclier*> m_TypeBonusBouclier;
	//! Dictionnaire des ennemis
	std::map<unsigned int, CTypeEnnemi*> m_TypeEnnemi;

	//! Recherche d'un bonus d'armes dans le dictionnaire d'armes
	CTypeBonusArme* GetTypeBonusArmeParIndex (unsigned int uiIndex) const;
	//! Recherche d'un bonus de boucliers dans le dictionnaire de boucliers
	CTypeBonusBouclier* GetTypeBonusBouclierParIndex (unsigned int uiIndex) const;
	//! Recherche d'un ennemi dans le dictionnaire des ennemis
	CTypeEnnemi* GetTypeEnnemiParIndex (unsigned int uiIndex) const;

	//! Parcours un élément 'jeu'
	void Jeu (xmlNode* pNode);
	//! Parcours un élement 'bonus-armes'
	void BonusArmes (xmlNode* pNode);
	//! Parcours un élément 'bonus-arme'
	std::pair<unsigned int, CTypeBonusArme*> BonusArme (xmlNode* pNode);
	//! Parcours un élément 'ancrage'
	glm::vec2 Ancrage (xmlNode* pNode);
	//! Parcours un élément 'bonus-boucliers'
	void BonusBoucliers (xmlNode* pNode);
	//! Parcours un élément 'bonus-bouclier'
	std::pair<unsigned int, CTypeBonusBouclier*> BonusBouclier (xmlNode* pNode);
	//! Parcours un élément 'ennemis'
	void Ennemis (xmlNode* pNode);
	//! Parcours un élément 'ennemi'
	std::pair<unsigned int, CTypeEnnemi*> Ennemi (xmlNode* pNode);
	//! Parcours un élément 'arme'
	CTypeArme* Arme (xmlNode* pNode);
	//! Parcours un élément 'niveaux'
	void Niveaux (xmlNode* pNode);
	//! Parcours un élément 'niveau'
	SNiveau Niveau (xmlNode* pNode);
	//! Parcours un élément 'index-armes'
	void IndexArmes (xmlNode* pNode, SNiveau & Niveau);
	//! Parcours un élément 'index-ennemis'
	void IndexEnnemis (xmlNode* pNode, SNiveau & Niveau);
	//! Parcours un élément 'index-boucliers'
	void IndexBoucliers (xmlNode* pNode, SNiveau & Niveau);
	//! Parcours les attributs d'un élément 'index-*'
	int IndexAttribute (xmlNode* pNode);

	//! Compte le nombre d'enfant d'un élément.
	unsigned int CompterChildren (xmlNode* pNode);
	//! Valide avec la DTD le document XML contenant les niveaux.
	bool Validation (xmlDoc* pDocument);

public:
	CXMLNiveau ();
	~CXMLNiveau ();

	//! Indique la durée d'un niveau.
	float GetTemps (unsigned int uiNiveau);
	//! Indique le nombre d'ennemis contenu dans le niveau identifié par 'uiNiveau'.
	unsigned int GetEnnemiNombre (unsigned int uiNiveau) const;
	//! indique le nombre de niveau total.
	unsigned int GetNiveauNombre () const;
	//! Indique l'intervale de temps entre deux création d'ennemis
	float GetEnnemiFrequence (unsigned int uiNiveau) const;
	//! Indique l'intervale de temps entre deux création de bonus d'armes
	float GetBonusArmeFrequence (unsigned int uiNiveau) const;
	//! Indique l'intervale de temps entre deux création de bonus de boucliers
	float GetBonusBouclierFrequence (unsigned int uiNiveau) const;

	//! Recherche 'aléatoirement' un type de bonus d'armes
	CTypeBonusArme* GetTypeBonusArme (unsigned int uiNiveau) const;
	//! Recherche 'aléatoirement' un type de bonus de boucliers
	CTypeBonusBouclier* GetTypeBonusBouclier (unsigned int uiNiveau) const;
	//! Recherche 'aléatoirement' un type d'ennemis
	CTypeEnnemi* GetTypeEnnemi (unsigned int uiNiveau) const;

	//! Chargement des niveaux
	bool Charger (const char* szFilename);
};

#endif //XML_NIVEAU_H
