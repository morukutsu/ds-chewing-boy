/* --------------------------- */
/* ITM 2                       */
/* --------------------------- */

//General entity File
//G�re tout les types d'�ntit�s que l'on va pouvoir trouver dans le jeu

#ifndef _ENTITY_GEMME_H
#define _ENTITY_GEMME_H

#include "Entity.h"
#include "../LowLevel/Platform.h"
#include "../LevelState.h"
#include "../WorldManager.h"

//Classes
class eGemme : public CEntity
{
public:
	//Destructeur
	eGemme();
    ~eGemme();
	//Fonctions g�n�rales
    void Draw(int layer);
    void Update();
    void Destroy();
    void Load();
    void CheckInteractions();
	//Fonctions d'action entit�s
	void Move(int move_type, int p1, int p2);

public:
	
	//Aspect Graphique
	LowLevel::Sprite mSpr, mFlare;
};

#endif