/* --------------------------- */
/* Hero                        */
/* --------------------------- */

//General entity File
//Gère tout les types d'éntités que l'on va pouvoir trouver dans le jeu

#ifndef _ENTITY_H
#define _ENTITY_H
#include "../LowLevel/Platform.h"
#include <string>

//Types de déplacements
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
	//Fonctions générales
    virtual void Draw(int layer) = 0;
    virtual void Update() = 0;
    virtual void Destroy() = 0;
    virtual void Load() = 0;
    virtual void CheckInteractions() = 0;
    void SetManager(WorldManagerOffline * wm) { worldMan = wm; }
	//Fonctions d'action entités
	virtual void Move(int move_type, int p1, int p2) = 0;

public:
    //Variables du manager
    WorldManagerOffline* worldMan;

    //Variables de position
	int id;
    int x, y;                  //Position x et y dans le monde
	int w, h;                  //Hauteur et largeur de l'entité
	int x_screen, y_screen;    //Position sur l'écran de l'entité
    int oldx, oldy;            //Ancienne position de l'entité dans le monde
    int vx, vy;                //Vecteur de direction instanné
	int oldvx, oldvy;
	int mMaxSpeedX;
	int mMaxSpeedXRun;
	int mMaxSpeedXAir;
	int mMaxSpeedXSlope;
	int mMaxVelJump;
	sHitTestPoints mHitPoints; //Pour les collisions avec la map
	sHitBoxRect mHitRect;      //Pour les collisions avec les entités
	int bodyID;

    //Variables de l'entité
    int type, genre;         //type et genre de l'entité
    int layer;               //Plan sur lequel est l'entité

    //Triggers logiques (disponibles pour toutes les entités)
	bool allowMove;		//Si l'entité est autorisée à se déplacer ou pas
	bool isOnTheFloor, wasInWater, isInWater;       //Si l'entité est au sol, sinon elle est en l'air
	bool mFallLeftSignal, mFallRightSignal;
	bool mWallAtLeft, mWallAtRight;
	bool isMoving;           //Si l'entité bouge ou est immobile
	bool mJumpPhase;
	bool mSens;

    //Concernant les interactions
    bool actif;      //Définit si l'objet est a l'écran ou pas
    bool toDelete;   //Il faut supprimer cet objet a la fin des opérations

	//Gestion de la gravité
	int density;    //Définit la densité de l'entité (densité de 256 = influence normale de la gravité)
	bool real;      //Définit si l'entité doit subir les collisions avec la map ou pas (+ update position)
	bool ignoreCollision; //Ingore la détection des collision, update seulement
};

#endif
