/* --------------------------- */
/* ITM 2                       */
/* --------------------------- */

//General entity File
//Gère tout les types d'éntités que l'on va pouvoir trouver dans le jeu

#ifndef _ENTITY_WOMIE_H
#define _ENTITY_WOMIE_H

#include "Entity.h"
#include "../LowLevel/Platform.h"
#include "../LevelState.h"
#include "../WorldManager.h"

//Classes
class eWomie : public CEntity
{
public:
	//Destructeur
	eWomie();
    ~eWomie();
	//Fonctions générales
    void Draw(int layer);
    void Update();
    void Destroy();
    void Load();
    void CheckInteractions();
	//Fonctions d'action entités
	void Move(int move_type, int p1, int p2);

public:
	//Déplacement
	int mGroundFriction;
	int mOldMoveType;
	int mThinkTimeX;
	bool mMovedX, mMovedRight, mMovedLeft, mXOrientation;
	bool mKeyControl;

	bool mMoveSens;

	//Hit
	int mDontHitTime;
	int mDontMoveTime;
	int mLife;

	int mDieTime;
	int mAngle;
	int mBounceTimes;
	int mFloorTimer;
	bool mProjectionSens;

	bool isInWater;

	//Aspect Graphique
	LowLevel::Sprite mSpr;
};

#endif