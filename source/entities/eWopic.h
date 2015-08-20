/* --------------------------- */
/* ITM 2                       */
/* --------------------------- */

//General entity File
//Gère tout les types d'éntités que l'on va pouvoir trouver dans le jeu

#ifndef _ENTITY_WOPIC_H
#define _ENTITY_WOPIC_H

#include "Entity.h"
#include "../LowLevel/Platform.h"
#include "../LevelState.h"
#include "../WorldManager.h"

//Classes
class eWopic : public CEntity
{
public:
	//Destructeur
	eWopic();
    ~eWopic();
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
	int mJumpTimer;
	int mLife;

	int mDieTime;
	int mAngle;
	int mBounceTimes;
	int mFloorTimer;
	bool mProjectionSens;

	//Aspect Graphique
	LowLevel::Sprite mSpr;
};

#endif