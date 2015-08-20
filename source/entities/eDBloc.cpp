/* --------------------------- */
/* ITM 2                       */
/* --------------------------- */

#include "eDBloc.h"

eDBloc::eDBloc()
{
	//Définition du type et du genre
	type = ENT_TYPE_OBJET;
	genre = ENT_OBJET_DBLOC;
	
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
	mHitRect.y1 = 0;
	mHitRect.x2 = 16;
	mHitRect.y2 = 16;

	//Renseignement du worldManager
	worldMan = LevelState::Instance()->worldMan;
	layer = 1;
}

eDBloc::~eDBloc()
{

}

void eDBloc::Load()
{
	//On demande la ressource du sprite
	mSpr.Assign(System::CRessourceManager::getInstance()->LoadImage("efs:/data/sprites/dbloc.png", UL_IN_VRAM, UL_PF_PAL4) );
	//On ajoute le body
	bodyID = worldMan->AddBody(0, 0, this, BODY_FULL);
	worldMan->mBodies[bodyID].aabb = &mHitRect;
	worldMan->mBodies[bodyID].isSensor = false;
	worldMan->mBodies[bodyID].isSleeping = false;
}

void eDBloc::Draw(int lay)
{
	//Affichage
	if(layer == lay && actif)
	{	
		mSpr.MoveTo(x_screen, y_screen);
		mSpr.Draw();
	}
}

void eDBloc::Update()
{
	actif = false;
	if(x_screen > -w && x_screen < 256+w && y_screen > -h && y_screen < 192+h)
	{
		actif = true;
		if(worldMan->mBodies[bodyID].collide)
		{
			for(int k = 0; k < worldMan->mBodies[bodyID].bodyContactCount; k++)
			{
				if(worldMan->mBodies[bodyID].with[k]->userdata == BODY_HERO)
				{
					if(worldMan->mHero->currentBall == 2)
					{
						toDelete = true;
						worldMan->mBodies[bodyID].toDelete = true;
						LowLevel::ParticleGenerator::getInstance()->AddGenerator(PARTICLES_SHORT_BOMBEXPLODE, fxtoi(x)+8, fxtoi(y)+8);
						LowLevel::SoundManager::getInstance()->PlaySoundSimple(SND_EXPLODE);
					}
				}
			}	
		}
	}
	//Calcul des coordonnées écran
	x_screen = fxtoi(x + worldMan->scrollX);
	y_screen = fxtoi(y + worldMan->scrollY);
}

void eDBloc::Destroy()
{

}

void eDBloc::CheckInteractions()
{

}

void eDBloc::Move(int move_type, int p1, int p2)
{

}