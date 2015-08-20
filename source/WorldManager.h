/* --------------------------- */
/* Star plot             */
/* --------------------------- */

//Includes
#ifndef _WORLDMANAGER_H
#define _WORLDMANAGER_H

#include "LowLevel/Platform.h"
#include "entities/Entities.h"
#include "entities/Entity.h"
#include <list>			

//Defines
//Caméra
#define CAMERA_FIXED       0
#define CAMERA_FOLLOW      1
#define CAMERA_SCRIPTED    2

//Events
#define EVENT_DESTROY      0
#define EVENT_ACTIVE       1
#define EVENT_DESACTIVE    3
#define EVENT_INVERT       4

//Bodies
#define BODY_FULL          0
#define BODY_HERO          1
#define BODY_HERO_HIT      2
#define BODY_PWUP          3
#define BODY_ENNEMY        4
#define BODY_WATER		   5

extern bool contactFilter[8][8];

static inline s32 LERP_Simple(s32 time, s32 time_start, s32 time_end, s32 value_start, s32 value_end)
{
	return (value_start + (((time - time_start) * (value_end - value_start)) / (time_end - time_start)));
}

static inline float LERP_Simplef(float time, float time_start, float time_end, float value_start, float value_end)
{
	return (value_start + (((time - time_start) * (value_end - value_start)) / (time_end - time_start)));
}

struct sEvent
{
	int eventType;             //Type d'event
	int entID;                 //ID de l'entité sur lequel appliquer la destruction
};

struct PenteInfo
{
	int v;   		           //Identifiant de tile
	u8 hauteur[16];            //Hauteur de chaque point
};

struct sPoint
{
	int x, y;
};

struct sParallaxC
{
	int x, y, w, spd;
	LowLevel::Sprite mSpr;
};


struct CBody
{
	//Param généraux
	int x, y;
	int shpx, shpy;
	int angle;
	int shape_type;  //AABB, Poly, Circle ?
	int userdata;
	bool toDelete;

	//Shapes
	sHitBoxRect* aabb;

	//Reponse à la collision
	bool collide, isFree, isSleeping, isSensor, isActive, isOnScreen;
	int bodyContactCount;
	CEntity *who;
	CBody* with[4];
};

class CEntity;
class eHero;
class WorldManagerOffline
{
public:
    void Init();                          //Initialisation du worldmanager
    void Cleanup();                       //Nettoyage du worldmanager
    void LoadMap(char* filename);         //Chargement d'un niveau
	void LoadMapScript(char* filename);   //Chargement du fichier script d'un niveau
	void LoadMapEntities(TiXmlDocument &doc); //Chargement des entités
	void LoadMapSlopes(char* filename);   //Chargement des pentes
    void FreeMap();                       //Vidage de la map
	void Reload();
	void ResetBeforeReload();
	
	void ProcessEntityPosition(CEntity* ent); //Calcule la nouvelle position de l'entité
	bool SlopePhase(CEntity* ent, int testx, int testy);
	bool SlopePhaseCheck(CEntity* ent, int testx, int testy);
	bool FlatPhase(CEntity* ent, int testx, int testy);
	bool WallPhase(CEntity* ent, int testx, int testy);
	bool PlafondPhase(CEntity* ent, int testx, int testy);
	bool StickPhase(CEntity* ent, int testx, int testy);
	bool FullPhase(CEntity* ent, int testx, int testy);
	void HandleBodyCollision();

    void LoadEntity(char* name, int x, int y, int w, int h, int p1, int p2, int id);
	void AddEntity(CEntity* entity);
	int  AddBody(int x, int y, CEntity* ent, int userdata = 0);
	
    void RegenWorld();                 //Regeneration du monde (après qu'une vie soit perdue)
	void FreeWorld();

    void Update();                     //Mise a jour du monde
	void GeneralScale(float factor);
    void Draw();                       //Affichage du monde

	void RumbleCamera(int frames);

	void ActivateBlackBar();
	void DesactivateBlackBar();

	bool IsOutsideOfWorld(int x, int y );   //Renvoie true si l'entité est en dehors du mode
	void PushEvent(int id, int eventType);
	CEntity* GetElementById(int id);

	void ShowSkyBG(int color);

public:
	
	//A : Ecran supérieur
		//Map
	LowLevel::Map mMap;
	PenteInfo mPentes[32];      //Infos sur les pentes de la map
	int mPentesTable[8][8];      //ID de pente en fonction de pID
	int mNumPentes;
	int mLevelGemmesCount, mLevelGemmesMax, mLifes;
	LowLevel::Sprite mGuiGem, mGuiBlob;

	sHitBoxRect aabbScreen;
	int scrollX, scrollY, oldScrollX, oldScrollY;
	int mCameraMode;
	eCamera mCam;
	
	eHero* mHero;               //Entité héros
	//CEntity* mFixedEntity;
	std::list<CEntity *> mEntities;
	std::list<CEntity *>::iterator mEntitiesItor;
	
		//Moteur Physique
	CBody mBodies[128];
	int mNextFreeBody;
	int mNextBody;
	int mEntryX, mEntryY;
	int mGravity, mGravityMax;
	int mNonSensorOnScreen[128];
	int mNonSensorOnScreenCount;
	bool isAnyNonSensorOnScreen;
	bool mLevelLoose;

		//Scripting
	int plop;

		//Effets en tout genre
	bool mSkyTransActivated;
	int mSkyTransTime, mCurrentSkyColorOrig, mCurrentSkyColor, mSkyTransAlpha, mSkyCloudsColor;
	float mSkyTransfTime;
	LowLevel::Sprite mBgStars;
	sParallaxC mParallaxClouds[16];
	int openScreenX;
	int fadeAlpha;
	bool activateFadeBlack, scrollFadeActivated, mLevelFinished, mTimeUp;
	int mFinishTimer;
	int mLevelTime, mLevelBest, mLevelID;

	int mFrameCounter;
};

#endif
