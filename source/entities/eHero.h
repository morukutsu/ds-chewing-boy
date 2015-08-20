/* --------------------------- */
/* ITM 2                       */
/* --------------------------- */

//General entity File
//G�re tout les types d'�ntit�s que l'on va pouvoir trouver dans le jeu

#ifndef _ENTITY_HERO_H
#define _ENTITY_HERO_H

#include "Entity.h"
#include "../LowLevel/Platform.h"

struct verletParticle
{
	float pX, pY, oX, oY, aX, aY;
};

struct previsionPoint
{
	int x, y, vx, vy;
};

//Classes
class eHero : public CEntity
{
public:
	//Destructeur
	eHero();
    ~eHero();
	//Fonctions g�n�rales
    void Draw(int layer);
    void Update();
    void Destroy();
    void Load();
    void CheckInteractions();
	//Fonctions d'action entit�s
	void Move(int move_type, int p1, int p2);
	
	void verlet();
	void accForces();
	void satConstraints();
	void checkColl();

public:
	LowLevel::Sprite mSpr, mPow, mPowArrow;
	int mPowBarAngle, mBarWidth;

	int curSpeed;
	int mMovingAngle;
	int vxBase, vyBase, vxDep, vyDep;
	int lastJumpPosX, lastJumpPosY, diex, diey;
	int mHeroAlpha, mDieTime;

	//Calcul de la trajectoire de saut
	int touchOriginX, touchOriginY, vdx, vdy, ivdx, ivdy, inorm;
	float norm;
	bool isDownHolding, isSticked, isFriction, isGrabbed, isThrowing, isReviving;
	float reviveLerp;

	//Gestion des diff�rentes ball
	int currentBall, powerBall;
	LowLevel::Image* ballImgs[4];

	//Pour la verlet/rope ball
	int mass, div, itr;
	float len;
	float massDiv, lenDiv, g, pm, dt;
	verletParticle parts[25];
	int partVx, partVy, partX, partY;
	float shootDist, pushPower;

	//Pour l'anticipation des trajectoires
	previsionPoint mPrevPoints[32];
	bool mComputingTraj;
	int mPrevisionLimit, mTrajAlpha;
};

#endif