/* --------------------------- */
/* ITM 2                       */
/* --------------------------- */

#include "eEnnemy.h"
#include "../LevelState.h"
#include "../WorldManager.h"
		
eEnnemy::eEnnemy()
{
	//Définition du type et du genre
	type = ENT_TYPE_ENNEMY;
	genre = ENT_ENNEMY_ENNEMY;
	
	//Paramatrage physique et hitbox
	density = 0;
	vx = vy = 0;
	real = true;
	toDelete = false;
	actif = true;
	w = 32;
	h = 32;
	x = y = 0;

	//Remplissage des coord. de hitbox
	mHitRect.mMirrored = false;
	mHitRect.x1 = 2;
	mHitRect.y1 = 2;
	mHitRect.x2 = 14;
	mHitRect.y2 = 14;
	
	//Renseignement du worldManager
	worldMan = LevelState::Instance()->worldMan;
	layer = 0;
}

eEnnemy::~eEnnemy()
{

}

void eEnnemy::Load()
{
	//On demande la ressource du sprite
	mSpr.Assign(System::CRessourceManager::getInstance()->LoadImage("efs:/data/sprites/ennemy_test.png", UL_IN_VRAM, UL_PF_PAL2) );
	mSpr.SetFrameWH(32, 32);
	//Ajout des différentes animations
	//mSpr.AddAnimation(0, 0, 6, 10);     //IDLE
	//mSpr.PlayAnim(ANIM_LOOP, 1);

	//On ajoute le body
	bodyID = worldMan->AddBody(0, 0, this, BODY_ENNEMY);
	worldMan->mBodies[bodyID].aabb = &mHitRect;
	worldMan->mBodies[bodyID].isSensor = true;
	worldMan->mBodies[bodyID].isSleeping = false;
}

void eEnnemy::Draw(int lay)
{
	//Affichage
	if(layer == lay && actif)
	{	
		//Sprite
		mSpr.Play();
		mSpr.MoveTo(x_screen, y_screen);
		mSpr.Draw();
	}
}

void eEnnemy::Update()
{
	//Calcul des coordonnées écran
	x_screen = fxtoi(x + worldMan->scrollX);
	y_screen = fxtoi(y + worldMan->scrollY);
}

void eEnnemy::Destroy()
{

}

void eEnnemy::CheckInteractions()
{

}

void eEnnemy::Move(int move_type, int p1, int p2)
{

}