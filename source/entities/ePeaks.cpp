/* --------------------------- */
/* ITM 2                       */
/* --------------------------- */

#include "ePeaks.h"

ePeaks::ePeaks()
{
	//Définition du type et du genre
	type = ENT_TYPE_ENNEMY;
	genre = ENT_ENNEMY_PEAKS;
	
	//Paramatrage physique et hitbox
	density = 0;
	vx = vy = 0;
	real = false;
	toDelete = false;
	w = 32;
	h = 32;
	ignoreCollision = true;

	//Remplissage des coord. de hitbox
	mHitRect.mMirrored = false;
	mHitRect.x1 = 0;
	mHitRect.y1 = 12;
	mHitRect.x2 = 16;
	mHitRect.y2 = 16;

	//Renseignement du worldManager
	worldMan = LevelState::Instance()->worldMan;
	layer = 0;
}

ePeaks::~ePeaks()
{

}

void ePeaks::Load()
{
	//On demande la ressource du sprite
	mSpr.Assign(System::CRessourceManager::getInstance()->LoadImage("efs:/data/sprites/peaks.png", UL_IN_VRAM, UL_PF_PAL4) );
	mSpr.Scale(16, 16);

	//Gestion de l'orientation
	if(orientation == 0)
	{
		//haut
		mHitRect.x1 = 0;
		mHitRect.y1 = 12;
		mHitRect.x2 = 16;
		mHitRect.y2 = 16;
		mSpr.Clip(0, 0, 16, 16);
	}
	else if(orientation == 1)
	{
		//bas
		mHitRect.x1 = 0;
		mHitRect.y1 = 0;
		mHitRect.x2 = 16;
		mHitRect.y2 = 4;
		mSpr.Clip(16, 0, 16, 16);
	}
	else if(orientation == 2)
	{
		//gauche
		mHitRect.x1 = 0;
		mHitRect.y1 = 0;
		mHitRect.x2 = 4;
		mHitRect.y2 = 16;
		mSpr.Clip(32, 0, 16, 16);
	}
	else if(orientation == 3)
	{
		//droite
		mHitRect.x1 = 12;
		mHitRect.y1 = 0;
		mHitRect.x2 = 16;
		mHitRect.y2 = 16;
		mSpr.Clip(48, 0, 16, 16);
	}

	//On ajoute le body
	bodyID = worldMan->AddBody(0, 0, this, BODY_ENNEMY);
	worldMan->mBodies[bodyID].aabb = &mHitRect;
	worldMan->mBodies[bodyID].isSensor = true;
	worldMan->mBodies[bodyID].isSleeping = false;
}

void ePeaks::Draw(int lay)
{
	//Affichage
	if(layer == lay && actif)
	{
		//Gestion de l'orientation
		if(orientation == 0)
			mSpr.Clip(0, 0, 16, 16);
		else if(orientation == 1)
			mSpr.Clip(16, 0, 16, 16);
		else if(orientation == 2)
			mSpr.Clip(32, 0, 16, 16);
		else if(orientation == 3)
			mSpr.Clip(48, 0, 16, 16);

		mSpr.MoveTo(x_screen, y_screen);
		mSpr.Draw();
	}
}

void ePeaks::Update()
{
	actif = true;

	//Calcul des coordonnées écran
	x_screen = fxtoi(x + worldMan->scrollX);
	y_screen = fxtoi(y + worldMan->scrollY);
}

void ePeaks::Destroy()
{

}

void ePeaks::CheckInteractions()
{

}

void ePeaks::Move(int move_type, int p1, int p2)
{

}