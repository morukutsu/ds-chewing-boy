/* --------------------------- */
/* ITM 2                       */
/* --------------------------- */

//General entity File
//Gère tout les types d'éntités que l'on va pouvoir trouver dans le jeu

#ifndef _ENTITY_ENNEMY_H
#define _ENTITY_ENNEMY_H

#include "Entity.h"
#include "../LowLevel/Platform.h"

//Classes
class eEnnemy : public CEntity
{
public:
	//Destructeur
	eEnnemy();
    ~eEnnemy();
	//Fonctions générales
    void Draw(int layer);
    void Update();
    void Destroy();
    void Load();
    void CheckInteractions();
	//Fonctions d'action entités
	void Move(int move_type, int p1, int p2);

public:
	LowLevel::Sprite mSpr;
};

#endif