/* --------------------------- */
/* Hero                        */
/* --------------------------- */

//General entity File
//G�re tout les types d'�ntit�s que l'on va pouvoir trouver dans le jeu

#ifndef _ENTITY_H
#define _ENTITY_H
#include "../LowLevel/Platform.h"
#include <string>

//Types de d�placements
#define MOVE_LEFT                0
#define MOVE_RIGHT               1
#define MOVE_JUMP                2
#define MOVE_ATTACK              3
#define MOVE_ATTACK2             4
#define MOVE_ATTACK3             5
#define MOVE_ATTACK4             6

//Classes
class WorldManagerOffline;
class CEntity
{
public:
	//Destructeur
    virtual ~CEntity() {};
	//Fonctions g�n�rales
    virtual void Draw(int layer) = 0;
    virtual void Update() = 0;
    virtual void Destroy() = 0;
    virtual void Load() = 0;
    virtual void CheckInteractions() = 0;
    void SetManager(WorldManagerOffline * wm) { worldMan = wm; }
	//Fonctions d'action entit�s
	virtual void Move(int move_type, int p1, int p2) = 0;

public:
    //Variables du manager
    WorldManagerOffline* worldMan;

    //Variables de position
	int id;
    int x, y;                  //Position x et y dans le monde
	int w, h;                  //Hauteur et largeur de l'entit�
	int x_screen, y_screen;    //Position sur l'�cran de l'entit�
    int oldx, oldy;            //Ancienne position de l'entit� dans le monde
    int vx, vy;                //Vecteur de direction instann�
	int oldvx, oldvy;
	int mMaxSpeedX;
	int mMaxSpeedXRun;
	int mMaxSpeedXAir;
	int mMaxSpeedXSlope;
	int mMaxVelJump;
	sHitTestPoints mHitPoints; //Pour les collisions avec la map
	sHitBoxRect mHitRect;      //Pour les collisions avec les entit�s
	int bodyID;

    //Variables de l'entit�
    int type, genre;         //type et genre de l'entit�
    int layer;               //Plan sur lequel est l'entit�

    //Triggers logiques (disponibles pour toutes les entit�s)
	bool allowMove;		//Si l'entit� est autoris�e � se d�placer ou pas
	bool isOnTheFloor, wasInWater, isInWater;       //Si l'entit� est au sol, sinon elle est en l'air
	bool mFallLeftSignal, mFallRightSignal;
	bool mWallAtLeft, mWallAtRight;
	bool isMoving;           //Si l'entit� bouge ou est immobile
	bool mJumpPhase;
	bool mSens;

    //Concernant les interactions
    bool actif;      //D�finit si l'objet est a l'�cran ou pas
    bool toDelete;   //Il faut supprimer cet objet a la fin des op�rations

	//Gestion de la gravit�
	int density;    //D�finit la densit� de l'entit� (densit� de 256 = influence normale de la gravit�)
	bool real;      //D�finit si l'entit� doit subir les collisions avec la map ou pas (+ update position)
	bool ignoreCollision; //Ingore la d�tection des collision, update seulement
};

#endif
