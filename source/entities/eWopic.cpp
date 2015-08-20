/* --------------------------- */
/* ITM 2                       */
/* --------------------------- */

#include "eWopic.h"

eWopic::eWopic()
{
	//Définition du type et du genre
	type = ENT_TYPE_ENNEMY;
	genre = ENT_ENNEMY_WOPIC;
	
	//Paramatrage physique et hitbox
	density = 256;
	vx = vy = 0;
	real = true;
	toDelete = false;
	w = 32;
	h = 32;
	ignoreCollision = false;
	mMaxSpeedXRun = mMaxSpeedX = 96;
	mMaxSpeedXSlope = 96;
	mOldMoveType = -1;
	mGroundFriction = 24;
	mThinkTimeX = 0;
	mMaxVelJump = 276;
	mKeyControl = false;
	mMoveSens = false;
	mDontHitTime = 0;
	mDontMoveTime = 0;
	mLife = 30;
	mDieTime = -1;
	mAngle = 0;
	mProjectionSens = false;
	mBounceTimes = 1;
	mFloorTimer = 0;
	mJumpTimer = 0;
	isInWater = false;
	mJumpPhase = false;

	//Triggers
	mMovedRight = false;
	mMovedLeft = false;
	
	//Remplissage des coord. de hitbox
	mHitPoints.mMirrored = false;
	mHitPoints.pTopX1 = 10, mHitPoints.pTopY1 = 18;
	mHitPoints.pBottomX = 7, mHitPoints.pBottomY = 31;
	mHitPoints.pTopSideX = 12, mHitPoints.pTopSideY = 20; 
	mHitPoints.pBottomSideX = 12, mHitPoints.pBottomSideY = 29; //28
	mHitPoints.pFallX1 = 10, mHitPoints.pFallY1 = 31;
	MirrorXHitTestPoints(15, true, &mHitPoints);
	
	mHitRect.mMirrored = false;
	mHitRect.x1 = 0;
	mHitRect.y1 = 0;
	mHitRect.x2 = 12;
	mHitRect.y2 = 18;

	//Renseignement du worldManager
	worldMan = LevelState::Instance()->worldMan;
	layer = 0;
}

eWopic::~eWopic()
{

}

void eWopic::Load()
{
	//On demande la ressource du sprite
	mSpr.Assign(System::CRessourceManager::getInstance()->LoadImage("efs:/data/sprites/spr_wopic.png", UL_IN_VRAM, UL_PF_PAL4) );
	//mSpr.Scale(32, 32);
	mSpr.SetFrameWH(16, 32);
	//Ajout des différentes animations
	mSpr.AddAnimation(0, 0, 2, 8);     //IDLE
	mSpr.PlayAnim(ANIM_LOOP, 1);
	mSpr.SetRotCenter(8, 24);
	//On ajoute le body
	bodyID = worldMan->AddBody(4, 14, this, BODY_ENNEMY);
	worldMan->mBodies[bodyID].aabb = &mHitRect;
	worldMan->mBodies[bodyID].isSensor = true;
	worldMan->mBodies[bodyID].isSleeping = false;
}

void eWopic::Draw(int lay)
{
	//Affichage
	if(layer == lay && actif)
	{
		if(mMoveSens == true)
			mSpr.Mirror(false, false);
		else
			mSpr.Mirror(true, false);
			
		mSpr.Play();
		mSpr.Rotate(mAngle);
		mSpr.MoveTo(x_screen+8, y_screen+24);
		mSpr.Draw();
	}
}

void eWopic::Update()
{
	//Calcul des coordonnées écran
	x_screen = fxtoi(x + worldMan->scrollX);
	y_screen = fxtoi(y + worldMan->scrollY);

	if(mDontHitTime != 0)
		mDontHitTime--;
	if(mDieTime != -1)
		mDieTime--;
	if(mDontMoveTime != 0)
		mDontMoveTime--;
	if(mJumpTimer != 0)
		mJumpTimer--;

	actif = false;
	if(x_screen > -w && x_screen < 256+w && y_screen > -h && y_screen < 192+h)
	{
		actif = true;
		if(worldMan->mBodies[bodyID].collide)
		{
			for(int k = 0; k < worldMan->mBodies[bodyID].bodyContactCount; k++)
			{
				if(worldMan->mBodies[bodyID].with[k]->userdata == BODY_HERO && worldMan->mBodies[bodyID].with[k]->isActive && mDontHitTime == 0)
				{
					if(worldMan->mHero->currentBall == 2)
					{
						LowLevel::ParticleGenerator::getInstance()->AddGenerator(PARTICLES_SHORT_EPINESEXPLODELOW, fxtoi(x)+8, fxtoi(y)+24);
						mDontHitTime = 30;
						mLife -= 15;
						mDontMoveTime = 20;
						LowLevel::SoundManager::getInstance()->PlaySoundSimple(SND_LASER);
					}
				}
			}	
		}
		//Animation de fin
		if(mDieTime != -1)
		{
			mAngle += 1024;

			if(mProjectionSens)
				vx = 256;
			else
				vx = -256;

			//Bounce
			if(mFloorTimer != 0)
				mFloorTimer--;
			if(isOnTheFloor && mFloorTimer == 0 && !isInWater)
			{
				mBounceTimes++;
				mFloorTimer = 5;
				vy = -512;
				LowLevel::ParticleGenerator::getInstance()->AddGenerator(PARTICLES_SHORT_SMOKE, fxtoi(x)+8, fxtoi(y)+24);
			}
		}
		if(mLife <= 0 && mDieTime == -1)
		{
			mDieTime = 120;
			vy = -768 + LowLevel::Rand(0, 128);
			mFloorTimer = 10;
			mProjectionSens = worldMan->mHero->mSens;
		}
		if(mDieTime == 120)
			worldMan->mBodies[bodyID].toDelete = true;
		//Gestion des déplacements
		LowLevel::Input input;
		if(mKeyControl == true)
		{
			 input = *LowLevel::InputManager::getInstance()->GetInput();
		}
		else
		{
			//IA de l'ennemi
			LowLevel::InputManager::getInstance()->Blank(&input);

			//Toutes les x frames on saute
			if(mJumpTimer == 0 && isOnTheFloor && mLife > 0)
			{
				input.butPressedY = true;
				mJumpTimer = 80;
				LowLevel::ParticleGenerator::getInstance()->AddGenerator(PARTICLES_SHORT_JSMOKE, fxtoi(x), fxtoi(y)+24);
				LowLevel::SoundManager::getInstance()->PlaySoundSimple(SND_JUMP);
			}

			//Inversion du sens
			if(x < worldMan->mHero->x)
				mMoveSens = true;
			if(x > worldMan->mHero->x)
				mMoveSens = false;
		}

		//Déplacement de eWopic
		//Horizontal
		mMovedX = false;
		if(mDontMoveTime == 0 && mDieTime == -1)
		{
			if(input.butPressedY)
			{
				Move(MOVE_JUMP, 0, 0);
			}
		}
	}
	
	if(mDieTime == 0 || mBounceTimes == 3)
	{
		toDelete = true;
		//worldMan->mBodies[bodyID].toDelete = true;
		if(actif)
		{
			LowLevel::ParticleGenerator::getInstance()->AddGenerator(PARTICLES_SHORT_EPINESEXPLODE, fxtoi(x)+8, fxtoi(y)+24);
			LowLevel::SoundManager::getInstance()->PlaySoundSimple(SND_EXPLODE);
		}
	}	
}

void eWopic::Destroy()
{

}

void eWopic::CheckInteractions()
{

}

void eWopic::Move(int move_type, int p1, int p2)
{
	//On essaie de savoir si deux mouvements opposés ont changé entre chaque frame
	//Déplacement
	if(move_type == MOVE_JUMP)
	{
		vy = -768;
	}

	mOldMoveType = move_type;
}