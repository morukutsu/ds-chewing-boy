/* --------------------------- */
/* ITM 2                       */
/* --------------------------- */

#include "eExtralife.h"

eExtralife::eExtralife()
{
	//Définition du type et du genre
	type = ENT_TYPE_OBJET;
	genre = ENT_OBJET_EXTRALIFE;
	
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
	mHitRect.x1 = 4;
	mHitRect.y1 = 4;
	mHitRect.x2 = 12;
	mHitRect.y2 = 12;

	//Renseignement du worldManager
	worldMan = LevelState::Instance()->worldMan;
	layer = 0;
}

eExtralife::~eExtralife()
{

}

void eExtralife::Load()
{
	//On demande la ressource du sprite
	mSpr.Assign(System::CRessourceManager::getInstance()->LoadImage("efs:/data/sprites/extralife.png", UL_IN_VRAM, UL_PF_PAL4) );
	mFlare.Assign(System::CRessourceManager::getInstance()->LoadImage("efs:/data/sprites/spr_flare_1.png", UL_IN_VRAM, UL_PF_PAL3_A5) );
	//On ajoute le body
	bodyID = worldMan->AddBody(0, 0, this, BODY_PWUP);
	worldMan->mBodies[bodyID].aabb = &mHitRect;
	worldMan->mBodies[bodyID].isSensor = true;
	worldMan->mBodies[bodyID].isSleeping = false;
}

void eExtralife::Draw(int lay)
{
	//Affichage
	if(layer == lay && actif)
	{	
		mFlare.MoveTo(x_screen-8, y_screen-8);
		mFlare.Draw();
		mSpr.MoveTo(x_screen, y_screen);
		mSpr.Draw();
	}
}

void eExtralife::Update()
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
					//On supprime la gemme
					toDelete = true;
					//worldMan->mLevelGemmesGetIDs[worldMan->mLevelGemmesCount] = id;
					worldMan->mLifes++;
					worldMan->mBodies[bodyID].toDelete = true;
					LowLevel::ParticleGenerator::getInstance()->AddGenerator(PARTICLES_SHORT_GEMGET, fxtoi(x)+8, fxtoi(y)+8);
					LowLevel::SoundManager::getInstance()->PlaySoundSimple(SND_DISAP);
				}
			}	
		}
	}
	//Calcul des coordonnées écran
	x_screen = fxtoi(x + worldMan->scrollX);
	y_screen = fxtoi(y + worldMan->scrollY);
}

void eExtralife::Destroy()
{

}

void eExtralife::CheckInteractions()
{

}

void eExtralife::Move(int move_type, int p1, int p2)
{

}