/* --------------------------- */
/* ITM 2                       */
/* --------------------------- */

#include "eHero.h"
#include "../LevelState.h"
#include "../WorldManager.h"
#include <math.h>

eHero::eHero()
{
	//Définition du type et du genre
	type = ENT_TYPE_HERO;
	genre = ENT_HERO_HERO;
	
	//Paramatrage physique et hitbox
	density = 256;
	vx = vy = 0;
	real = true;
	allowMove = true;
	toDelete = false;
	actif = true;
	ignoreCollision = true;
	w = 32;
	h = 32;
	x = y = 0;
	mHeroAlpha = 31;
	mComputingTraj = false;
	isOnTheFloor = false;
	isFriction = false;
	isGrabbed = false;
	isThrowing = false;
	isSticked = false;
	isReviving = false;
	mTrajAlpha = 0;
	curSpeed = 256;
	powerBall = 100<<8;
	mDieTime = -1;

	//Remplissage des coord. de hitbox
	mHitRect.mMirrored = false;
	mHitRect.x1 = 2;
	mHitRect.y1 = 2;
	mHitRect.x2 = 14;
	mHitRect.y2 = 14;
	
	//Renseignement du worldManager
	worldMan = LevelState::Instance()->worldMan;
	layer = 0;
	mPowBarAngle = mBarWidth = 0;

	//Gestion des balls
	currentBall = 0; //0 basic, 1 stick, 2 fire, 3 verlet
	mTrajAlpha = itofx(31);
	mPrevisionLimit = 0;
	//Ressorts
	mass = 6;
	len = 7;
	div = 10;
	g = 0.25f;
	dt = 1.0f / 60.0f;

	for(int i = 0; i <= div; i++) 
	{
		parts[i].pX = 0;
		parts[i].pY = 0;
		parts[i].oX = 0;
		parts[i].oY = 0;
		parts[i].aX = 0;
		parts[i].aY = 0;
	}
	
}

eHero::~eHero()
{

}

void eHero::Load()
{
	//On demande la ressource du sprite
	mSpr.Assign(System::CRessourceManager::getInstance()->LoadImage("efs:/data/sprites/jello_sheet.png", UL_IN_VRAM, UL_PF_PAL4) );
	mPow.Assign(System::CRessourceManager::getInstance()->LoadImage("efs:/data/sprites/pow_arrow.png", UL_IN_VRAM, UL_PF_PAL4) );
	mPowArrow.Assign(System::CRessourceManager::getInstance()->LoadImage("efs:/data/sprites/pow_arrow_1.png", UL_IN_VRAM, UL_PF_PAL2) );
	
	//Preloading
	ballImgs[2] = System::CRessourceManager::getInstance()->LoadImage("efs:/data/sprites/jello_sheet_fire.png", UL_IN_VRAM, UL_PF_PAL4);
	ballImgs[1] = System::CRessourceManager::getInstance()->LoadImage("efs:/data/sprites/jello_sheet_stick.png", UL_IN_VRAM, UL_PF_PAL4);
	ballImgs[3] = System::CRessourceManager::getInstance()->LoadImage("efs:/data/sprites/jello_sheet_verlet.png", UL_IN_VRAM, UL_PF_PAL4);
	ballImgs[0] = System::CRessourceManager::getInstance()->LoadImage("efs:/data/sprites/jello_sheet.png", UL_IN_VRAM, UL_PF_PAL4);

	//On ajoute le body
	bodyID = worldMan->AddBody(0, 0, this, BODY_HERO);
	worldMan->mBodies[bodyID].aabb = &mHitRect;
	worldMan->mBodies[bodyID].isSensor = true;
	worldMan->mBodies[bodyID].isSleeping = false;
	
	//Sprite principal
	mSpr.AddAnimation(0, 0, 3, 8); //Idle
	mSpr.AddAnimation(1, 0, 3, 4); //Down hold
	mSpr.AddAnimation(2, 0, 3, 4); //jump up
	mSpr.AddAnimation(3, 0, 2, 4); //jump down

	mSpr.SetFrameWH(16, 16);
	mSpr.Scale(16, 16);

	mSpr.PlayAnim(ANIM_LOOP, 1);

	//Flèche & barre
	mPowBarAngle = mBarWidth = 0;
}

void eHero::Draw(int lay)
{
	//Affichage
	if(layer == lay && actif && !isReviving)
	{	
		//Affichage trajectoire
		if(mTrajAlpha > 256 && currentBall != 3)
		{
			for(int k = 1; k < mPrevisionLimit; k++)
				LowLevel::drawRect(fxtoi(mPrevPoints[k].x + worldMan->scrollX), fxtoi(mPrevPoints[k].y + worldMan->scrollY), 
				fxtoi(mPrevPoints[k].x + worldMan->scrollX) + 2, fxtoi(mPrevPoints[k].y + worldMan->scrollY) + 2, 0, mTrajAlpha>>8);
		}

		//Barre
		mPow.MoveTo(x_screen+8, y_screen+8);
		mPow.SetRotCenter(0, 8);
		mPow.Clip(0, 0, mBarWidth, 16);
		if(mBarWidth != 0 && isDownHolding)
		{
			mPow.Scale(mBarWidth, 16);
			mPow.Rotate(mPowBarAngle);
			mPow.Draw();
		}

		//Flèche
		if(mBarWidth != 0 && isDownHolding)
		{
			mPowArrow.SetRotCenter(8, 8);
			mPowArrow.MoveTo(x_screen+8 + fxtoi(Mulfx(itofx(mBarWidth+4), cosLerp(mPowBarAngle)>>4) - Mulfx(itofx(0), sinLerp(mPowBarAngle)>>4)), 
				y_screen+8 + fxtoi(Mulfx(itofx(mBarWidth+4), sinLerp(mPowBarAngle)>>4) + Mulfx(itofx(0), cosLerp(mPowBarAngle)>>4)));
			mPowArrow.Rotate(mPowBarAngle);
			mPowArrow.Draw();
		}
		
		//Sprite
		mSpr.Scale(16, 16);
		mSpr.Play();
		mSpr.MoveTo(x_screen, y_screen);
		mSpr.Alpha(mHeroAlpha, 1);
		mSpr.Draw();
		
		//Tir verlet ball
		if(isThrowing)
		{
			ulDrawLine(fxtoi(partX + worldMan->scrollX), fxtoi(partY + worldMan->scrollY), 
				x_screen+7, y_screen+7, RGB15(0, 0, 0));
			LowLevel::drawRect(fxtoi(partX + worldMan->scrollX), fxtoi(partY + worldMan->scrollY), 
				fxtoi(partX + worldMan->scrollX) + 2, fxtoi(partY + worldMan->scrollY) + 2, RGB15(31, 31, 0));	
		}

		//mPowBarAngle += 32;

		//Affichage
		if(isGrabbed)
		{
			for(int k = 1; k <= div; k++)
			{
				ulDrawLine(parts[k-1].pX + fxtoi(worldMan->scrollX), parts[k-1].pY + fxtoi(worldMan->scrollY), 
					parts[k].pX + fxtoi(worldMan->scrollX), parts[k].pY + fxtoi(worldMan->scrollY), RGB15(0, 0, 0));
			}

			for(int k = 0; k <= div; k++)
				LowLevel::drawRect(parts[k].pX + fxtoi(worldMan->scrollX), parts[k].pY + fxtoi(worldMan->scrollY), 
				parts[k].pX + fxtoi(worldMan->scrollX) + 2, parts[k].pY + fxtoi(worldMan->scrollY) + 2 , RGB15(31, 31, 0));
		}
	}
}

void eHero::Update()
{
	//Initialisations de frame
	LowLevel::Input* input;
	if(allowMove)
		input = LowLevel::InputManager::getInstance()->GetInput();
	else
		LowLevel::InputManager::getInstance()->Blank(input);

	//Calcul des coordonnées écran
	x_screen = fxtoi(x + worldMan->scrollX);
	y_screen = fxtoi(y + worldMan->scrollY);

	//Déplacement du blob
	isDownHolding = false;
	if(input->butPressedTouch && !isGrabbed && !isThrowing)
	{
		touchOriginX = input->touchX;
		touchOriginY = input->touchY;
		//isGrabbed = false;
		//isThrowing = false;
		shootDist = 0.0f;
	}

	//Hitboxes
	if(currentBall == 2)
	{
		mHitRect.x1 = 0;
		mHitRect.y1 = 0;
		mHitRect.x2 = 16;
		mHitRect.y2 = 16;
	}
	else
	{
		mHitRect.x1 = 2;
		mHitRect.y1 = 2;
		mHitRect.x2 = 14;
		mHitRect.y2 = 14;
	}

	if(input->butHeldTouch && !isGrabbed && !isThrowing)
	{
		vdx = input->touchX-touchOriginX;
		vdy = input->touchY-touchOriginY;
		norm = sqrt(vdx*vdx + vdy*vdy);
		ivdx = ftofx(vdx/norm);
		ivdy = ftofx(vdy/norm);
		
		inorm = ftofx(norm/20);
		if(inorm >= itofx(4))
			inorm = itofx(4);
		
		mBarWidth = (int)(35 * (inorm/1024.0f));
		mPowBarAngle = LowLevel::GetAngle2Points(input->touchX, input->touchY, touchOriginX, touchOriginY)-8192;
		isDownHolding = true;

		if(currentBall != 3)
		{
			if(isOnTheFloor || isSticked)
			{
				//Calcul des trajectoires
				mTrajAlpha = itofx(31);
				int oldX, oldY, oldvX, oldvY, curX, curY, curvX, curvY;

				curX = x + itofx(7);
				curY = y + itofx(7);
				curvX = Mulfx(ivdx, inorm);
				curvY = Mulfx(ivdy, inorm);

				for(int k = 0; k < 32; k++)
				{
					for(int i = 0; i < 3; i++)
					{
						curX += curvX;
						
						if(isSticked)
							density = 256;

						curvY += Mulfx(worldMan->mGravity, density);
						if(curvY >= Mulfx(worldMan->mGravityMax, density))
							curvY = Mulfx(worldMan->mGravityMax, density);
						curY += curvY;

						if(isSticked)
							density = 0;
						
						mPrevPoints[k].x = curX;
						mPrevPoints[k].y = curY;
					}
					
					mPrevisionLimit = k;
					if(worldMan->FlatPhase(this, curX>>8, curY>>8) || worldMan->SlopePhaseCheck(this, curX>>8, curY>>8) )
						break;
				}
			}
		}
	}

	if(input->butReleasedTouch && !isGrabbed && !isThrowing)
	{
		if(currentBall != 3)
		{
			if(isOnTheFloor || isSticked)
			{
				vx = Mulfx(ivdx, inorm);
				vy = Mulfx(ivdy, inorm);
				
				mBarWidth = 0;

				if(currentBall == 1)
					isSticked = false;

				if(currentBall == 0)
				{
					lastJumpPosX = x;
					lastJumpPosY = y;
				}
			}
		}
		else
		{
			int angleTir = 8192-LowLevel::GetAngle2Points(Mulfx(ivdx, inorm), Mulfx(ivdy, inorm), 0, 0);
			isThrowing = true;
			mBarWidth = 0;
			len = 7;
			partVx = Mulfx(cosLerp(angleTir)>>4, 1024);
			partVy = -Mulfx(sinLerp(angleTir)>>4, 1024);
			partX = x+itofx(7);
			partY = y+itofx(7);
		}	

		LowLevel::SoundManager::getInstance()->PlaySoundSimple(SND_JUMP);
	}
	
	if(isThrowing)
	{
		//Update
		partX += partVx;
		partY += partVy;
		int dx = x - partX;
		int dy = y - partY;
		shootDist = sqrt((dx>>8)*(dx>>8) + (dy>>8)*(dy>>8));
		if(shootDist >= len*(div+1))
		{
			isThrowing = false;
		}

		//Collision
		if(worldMan->FullPhase(this, partX>>8, partY>>8) )
		{
			isThrowing = false;

			//On active la corde
			isGrabbed = true;
			parts[div].pX = parts[div].oX = partX>>8;
			parts[div].pY = parts[div].oY = partY>>8;
			len = shootDist/(div+1);
			float interp = 0.0f;
			for(int i = 0; i <= div-1; i++) 
			{
				parts[i].pX = LERP_Simplef(interp, 0.0f, 1.0f, (x>>8), partX>>8);
				parts[i].pY = LERP_Simplef(interp, 0.0f, 1.0f, (y>>8), partY>>8);
				parts[i].oX = parts[i].pX;
				parts[i].oY = parts[i].pY;
				parts[i].aX = 0;
				parts[i].aY = g;

				interp += 0.1f;
			}
			parts[0].aX = (vx/256)*2;
			parts[0].aY += (vy/256)*2;
		}
		
		
	}

	//Changement de ball
	if(isGrabbed)
	{
		if(input->butHeldUp || input->butHeldX)
		{
			len -= 0.05f;
		}
		if(input->butHeldDown || input->butHeldB)
		{
			len += 0.05f;
		}
		if(len >= 7)
			len = 7;
		if(len <= 3)
			len = 3;
		
		if(input->butPressedLeft || input->butPressedY || input->butPressedRight || input->butPressedA)
		{
			pushPower = 1.0f;
		}

		if(input->butHeldLeft || input->butHeldY)
		{
			parts[0].aX = -0.5f*pushPower;
			pushPower -= 0.025f;
			if(pushPower <= 0.0f)
				pushPower = 0.0f;
		}
		else if(input->butHeldRight || input->butHeldA)
		{
			parts[0].aX = 0.5f*pushPower;
			pushPower -= 0.025f;
			if(pushPower <= 0.0f)
				pushPower = 0.0f;
		}
		else
			parts[0].aX = 0.0f;

		//Annulation de la ball
		if(input->butPressedL || input->butPressedR)
		{
			//currentBall = 0; // Normal
			isSticked = false;
			isGrabbed = false;
			isThrowing = false;
			//density = 256;
			//mSpr.Assign(ballImgs[currentBall]);
			vx = (parts[0].pX - parts[0].oX)*256;
			vy = -256+(parts[0].pY - parts[0].oY)*256;
			LowLevel::SoundManager::getInstance()->PlaySoundSimple(SND_BACK);
		}
	}
	else if(isOnTheFloor || isSticked)
	{
		if(input->butPressedUp || input->butPressedX)
		{
			currentBall = 0; // Normal
			isSticked = false;
			isGrabbed = false;
			isThrowing = false;
			density = 256;
			mSpr.Assign(ballImgs[currentBall]);
			LowLevel::SoundManager::getInstance()->PlaySoundSimple(SND_BACK);
		}
		if(input->butPressedRight || input->butPressedA)
		{
			currentBall = 2; // Fire
			isSticked = false;
			isGrabbed = false;
			isThrowing = false;
			density = 512;
			mSpr.Assign(ballImgs[currentBall]);
			LowLevel::SoundManager::getInstance()->PlaySoundSimple(SND_BACK);
		}
		if(input->butPressedDown || input->butPressedDown)
		{
			currentBall = 1; // Stick
			density = 256;
			isGrabbed = false;
			isThrowing = false;
			mSpr.Assign(ballImgs[currentBall]);
			LowLevel::SoundManager::getInstance()->PlaySoundSimple(SND_BACK);
		}
		if(input->butPressedLeft || input->butPressedLeft)
		{
			currentBall = 3; // Verlet
			density = 128;
			mSpr.Assign(ballImgs[currentBall]);
			LowLevel::SoundManager::getInstance()->PlaySoundSimple(SND_BACK);
		}
	}
	
	if(isGrabbed)
	{
		parts[div].pX = partX>>8;
		parts[div].pY = partY>>8;
		
		//Positionnement de la ball
		x = parts[0].pX*256-itofx(7);
		y = parts[0].pY*256-itofx(7);

		accForces();
		verlet();
		
		parts[div].pX = partX>>8;
		parts[div].pY = partY>>8;

		for(int j = 0; j <= div; j++)
		{
			satConstraints();
		}
		checkColl();
	}

	//Alpha
	if(mHeroAlpha < 31 && mDieTime == -1)
		mHeroAlpha++;

	//Ecoulement de l'energie des pouvoirs
	if(powerBall <= 0)
	{
		currentBall = 0; // Normal
		isSticked = false;
		density = 256;
		mSpr.Assign(ballImgs[currentBall]);
		isGrabbed = false;
	}
	if(powerBall >= itofx(100) )
		powerBall = itofx(100);
	if(currentBall == 1)
	{
		powerBall -= 32;
	}
	else if(currentBall == 2)
	{
		powerBall -= 192;
	}
	else if(currentBall == 3)
	{
		powerBall -= 32;
	}
	else
	{
		if(isOnTheFloor)
			powerBall += 64;
	}
	
	//Friction
	if(isFriction)
	{
		if(vx <= -32)
			vx += 31;
		if(vx >= 32)
			vx -= 31;

		if(vx > -32 && vx < 32)
			vx = 0;
	}

	//Détection des collisions
	//Carré de détection
	//      Flat
	//  Wall    Wall
	//      Flat
	// 
	if(currentBall == 0 || currentBall == 2)
	{
		//Basic ball
		//Bas
		isOnTheFloor = false;
		isFriction = false;
		isGrabbed = false;
		isSticked = false;
		if(vy >= 0)
		{
			mHitPoints.pBottomY = 15;
			bool slopeFound = false;

			//Creation des points de tests
			sPoint mTestPoints[14];
			int testx = ((x+vx)>>8)+8;
			int testy = ((y+vy)>>8)+14;
			mTestPoints[0].x = testx, mTestPoints[0].y = testy;
			mTestPoints[1].x = testx, mTestPoints[1].y = testy-1;

			mTestPoints[2].x = testx-1, mTestPoints[2].y = testy;
			mTestPoints[3].x = testx+1, mTestPoints[3].y = testy;

			mTestPoints[4].x = testx-1, mTestPoints[4].y = testy-1;
			mTestPoints[5].x = testx+1, mTestPoints[5].y = testy-1;

			mTestPoints[6].x = testx-1, mTestPoints[6].y = testy-2;
			mTestPoints[7].x = testx+1, mTestPoints[7].y = testy-2;
			
			for(int k = 0; k < 8; k++)
			{
				slopeFound = worldMan->SlopePhase(this, mTestPoints[k].x, mTestPoints[k].y);
				if(slopeFound)
				{
					vy = 0; 
					isOnTheFloor = true;
					isFriction = true;
					break;
				}
			}
			
			if(!slopeFound)
			{
				if(worldMan->FlatPhase(this, ((x+vx)>>8)+8, ((y+vy)>>8)+15))
				{
					vy = 0; 
					isOnTheFloor = true;
					isFriction = true;
				}
			}
		}
		else if(vy < 0)
		{
			//Haut
			if(worldMan->FlatPhase(this, ((x+vx)>>8)+8, ((y+vy)>>8)+4) || worldMan->SlopePhaseCheck(this, ((x+vx)>>8)+8, ((y+vy)>>8)+4))
				vy = 0;
		}
		
		//Gauche & droite
		/*if(worldMan->FlatPhase(this, ((x+vx)>>8)+7, ((y+vy)>>8)+8) || worldMan->SlopePhaseCheck(this, ((x+vx)>>8)+7, ((y+vy)>>8)+8))
			vx = 0;
		if(worldMan->FlatPhase(this, ((x+vx)>>8)+9, ((y+vy)>>8)+8) || worldMan->SlopePhaseCheck(this, ((x+vx)>>8)+9, ((y+vy)>>8)+8))
			vx = 0;*/
		if(worldMan->FlatPhase(this, ((x+vx)>>8)+7, ((y+vy)>>8)+8) || worldMan->FlatPhase(this, ((x+vx)>>8)+7, ((y+vy)>>8)+12)
				|| worldMan->SlopePhaseCheck(this, ((x+vx)>>8)+7, ((y+vy)>>8)+8))
				vx = 0;
		if(worldMan->FlatPhase(this, ((x+vx)>>8)+9, ((y+vy)>>8)+8) || worldMan->FlatPhase(this, ((x+vx)>>8)+9, ((y+vy)>>8)+12)
				|| worldMan->SlopePhaseCheck(this, ((x+vx)>>8)+9, ((y+vy)>>8)+8))
				vx = 0;
	}
	else if(currentBall == 1)
	{
		//Stick ball
		//On teste si le pixel du bas est un stick ou no stick
		isFriction = false;
		isGrabbed = false;
		isOnTheFloor = false;
		if(worldMan->StickPhase(this, ((x+vx)>>8)+8, ((y+vy)>>8)+15) )
		{
			//Le pixel est rouge (4) donc no stick slope/bloc
			isOnTheFloor = false;
			isSticked = false;
			if(vy >= 0)
			{
				mHitPoints.pBottomY = 15;
				bool slopeFound = false;

				//Creation des points de tests
				sPoint mTestPoints[14];
				int testx = ((x+vx)>>8)+8;
				int testy = ((y+vy)>>8)+14;
				mTestPoints[0].x = testx, mTestPoints[0].y = testy;
				mTestPoints[1].x = testx, mTestPoints[1].y = testy-1;

				mTestPoints[2].x = testx-1, mTestPoints[2].y = testy;
				mTestPoints[3].x = testx+1, mTestPoints[3].y = testy;

				mTestPoints[4].x = testx-1, mTestPoints[4].y = testy-1;
				mTestPoints[5].x = testx+1, mTestPoints[5].y = testy-1;

				mTestPoints[6].x = testx-1, mTestPoints[6].y = testy-2;
				mTestPoints[7].x = testx+1, mTestPoints[7].y = testy-2;
				
				for(int k = 0; k < 8; k++)
				{
					slopeFound = worldMan->SlopePhase(this, mTestPoints[k].x, mTestPoints[k].y);
					if(slopeFound)
					{
						vy = 0; 
						isOnTheFloor = true;
						isFriction = true;
						break;
					}
				}

				if(!slopeFound)
				{
					if(worldMan->FlatPhase(this, ((x+vx)>>8)+8, ((y+vy)>>8)+15))
					{
						vy = 0; 
						isOnTheFloor = true;
						isFriction = true;
					}
				}
			}
			else if(vy < 0)
			{
				//Haut
				if(worldMan->FlatPhase(this, ((x+vx)>>8)+8, ((y+vy)>>8)+4) || worldMan->SlopePhaseCheck(this, ((x+vx)>>8)+8, ((y+vy)>>8)+4))
					vy = 0;
			}
			
			//Gauche & droite
			if(worldMan->FlatPhase(this, ((x+vx)>>8)+7, ((y+vy)>>8)+8)
				|| worldMan->SlopePhaseCheck(this, ((x+vx)>>8)+7, ((y+vy)>>8)+8))
				vx = 0;
			if(worldMan->FlatPhase(this, ((x+vx)>>8)+9, ((y+vy)>>8)+8)
				|| worldMan->SlopePhaseCheck(this, ((x+vx)>>8)+9, ((y+vy)>>8)+8))
				vx = 0;
		}
		else
		{
			//Pixel stick
			if(worldMan->FullPhase(this, ((x+vx)>>8)+8, ((y+vy)>>8)+14) || worldMan->FullPhase(this, ((x+vx)>>8)+8, ((y+vy)>>8)+7) )
			{
				vx = vy = 0;
				isOnTheFloor = false, isSticked = true;
				isFriction = false;
			}
		}
		
		//Gestion arrêt du mouvement & gravité
		if(isSticked)
			density = 0;
		else
			density = 256;
	}
	else if(currentBall == 3)
	{
		//Basic ball
		//Bas
		isOnTheFloor = false;
		isFriction = false;
		if(vy >= 0)
		{
			mHitPoints.pBottomY = 15;
			bool slopeFound = false;

			//Creation des points de tests
			sPoint mTestPoints[14];
			int testx = ((x+vx)>>8)+8;
			int testy = ((y+vy)>>8)+14;
			mTestPoints[0].x = testx, mTestPoints[0].y = testy;
			mTestPoints[1].x = testx, mTestPoints[1].y = testy-1;

			mTestPoints[2].x = testx-1, mTestPoints[2].y = testy;
			mTestPoints[3].x = testx+1, mTestPoints[3].y = testy;

			mTestPoints[4].x = testx-1, mTestPoints[4].y = testy-1;
			mTestPoints[5].x = testx+1, mTestPoints[5].y = testy-1;

			mTestPoints[6].x = testx-1, mTestPoints[6].y = testy-2;
			mTestPoints[7].x = testx+1, mTestPoints[7].y = testy-2;
			
			for(int k = 0; k < 8; k++)
			{
				slopeFound = worldMan->SlopePhase(this, mTestPoints[k].x, mTestPoints[k].y);
				if(slopeFound)
				{
					vy = 0; 
					isOnTheFloor = true;
					isFriction = true;
					break;
				}
			}
			
			if(!slopeFound)
			{
				if(worldMan->FlatPhase(this, ((x+vx)>>8)+8, ((y+vy)>>8)+15))
				{
					vy = 0; 
					isOnTheFloor = true;
					isFriction = true;
				}
			}
		}
		else if(vy < 0)
		{
			//Haut
			if(worldMan->FlatPhase(this, ((x+vx)>>8)+8, ((y+vy)>>8)+4) || worldMan->SlopePhaseCheck(this, ((x+vx)>>8)+8, ((y+vy)>>8)+4))
				vy = 0;
		}
		
		//Gauche & droite
		if(worldMan->FlatPhase(this, ((x+vx)>>8)+7, ((y+vy)>>8)+8) || worldMan->FlatPhase(this, ((x+vx)>>8)+7, ((y+vy)>>8)+12)
				|| worldMan->SlopePhaseCheck(this, ((x+vx)>>8)+7, ((y+vy)>>8)+8))
				vx = 0;
		if(worldMan->FlatPhase(this, ((x+vx)>>8)+9, ((y+vy)>>8)+8) || worldMan->FlatPhase(this, ((x+vx)>>8)+9, ((y+vy)>>8)+12)
				|| worldMan->SlopePhaseCheck(this, ((x+vx)>>8)+9, ((y+vy)>>8)+8))
				vx = 0;
	}
	
	//Gestion fin de vie
	if(y > itofx(worldMan->mMap.mMapHeight<<4) && !isReviving && mDieTime == -1)
	{
		diex = x;
		diey = y;
		/*x = lastJumpPosX;
		y = lastJumpPosY;*/
		vx = 0;
		vy = 0;
		isReviving = true;
		reviveLerp = 0.0f;
		allowMove = false;
		worldMan->mLifes--;
	}

	//Collision avec un ennemi
	if(worldMan->mBodies[bodyID].collide && currentBall != 2 && !isReviving)
	{
		for(int k = 0; k < worldMan->mBodies[bodyID].bodyContactCount; k++)
		{
			if(worldMan->mBodies[bodyID].with[k]->userdata == BODY_ENNEMY && mDieTime == -1)
			{
				currentBall = 0; // Normal
				isSticked = false;
				density = 256;
				mSpr.Assign(ballImgs[currentBall]);
				isGrabbed = false;
				worldMan->mLifes--;

				allowMove = false;
				if(vx <= 256)
				{
					vx = 256;
				}
				else if(vx >= 256)
				{
					vx = -256;
				}
				else
				{
					vx = 0;
				}
				
				vy = -784;
				mDieTime = 0;
				LowLevel::ParticleGenerator::getInstance()->AddGenerator(PARTICLES_SHORT_EPINESEXPLODELOW, fxtoi(x)+7, fxtoi(y)+7);
				LowLevel::SoundManager::getInstance()->PlaySoundSimple(SND_LASER);
			}
		}
	}
	
	if(worldMan->mLifes < 0)
	{
		worldMan->mLevelLoose = true;
	}

	if(mDieTime != -1)
	{	
		mDieTime++;
		if(mDieTime >= 90)
			mHeroAlpha--;
		if(mHeroAlpha <= 1)
		{
			diex = x;
			diey = y;
			vx = 0;
			vy = 0;
			reviveLerp = 0.0f;
			allowMove = false;
			isReviving = true;
			mDieTime = -1;	
		}	
	}

	if(isReviving)
	{
		x = LERP_Simplef(reviveLerp, 0.0f, 1.0f, diex, lastJumpPosX);
		y = LERP_Simplef(reviveLerp, 0.0f, 1.0f, diey, lastJumpPosY);
		reviveLerp += 0.05f;
		if(reviveLerp >= 1.0f)
		{
			isReviving = false;
			x = lastJumpPosX;
			y = lastJumpPosY;
			currentBall = 0; // Normal
			isSticked = false;
			density = 256;
			mSpr.Assign(ballImgs[currentBall]);
			isGrabbed = false;
			mHeroAlpha = 1;
			mDieTime = -1;
			allowMove = true;
		}
	}

	//Changement animations
	if(isOnTheFloor || isSticked)
	{
		if(isDownHolding)
			mSpr.PlayAnim(ANIM_ONCE, 2);
		else
			mSpr.PlayAnim(ANIM_LOOP, 1);
	}
	else
	{
		if(vy < 0)
			mSpr.PlayAnim(ANIM_ONCE, 3);
		else
			mSpr.PlayAnim(ANIM_ONCE, 4);
	}

	//Changement statuts déplacement
	if(!isOnTheFloor)
		isDownHolding = false;
	
	//Modifs trajectoire
	if(mTrajAlpha > 0)
		mTrajAlpha -= 128;

	//Sortie écran
	if(fxtoi(x + vx) < 0)
		vx = 0;
	if(fxtoi(x + vx) > worldMan->mMap.mMapWidth*16+16)
		vx = 0;
	if(fxtoi(y + vy) < 0)
		vy = 0;
	if(fxtoi(y + vy) > worldMan->mMap.mMapHeight*16+16)
		vy = 0;
}

void eHero::Destroy()
{

}

void eHero::CheckInteractions()
{

}

void eHero::Move(int move_type, int p1, int p2)
{

}

void eHero::verlet()
{
	for(int i = 0; i <= div; i++) 
	{
		float tempX = parts[i].pX;
		//parts[i].pX += (0.99*parts[i].pX - 0.99*parts[i].oX) + (parts[i].aX/* * dt * dt*/);
		parts[i].pX += (parts[i].pX - parts[i].oX) + (parts[i].aX/* * dt * dt*/);
		float tempY = parts[i].pY;
		//parts[i].pY += (0.99*parts[i].pY - 0.99*parts[i].oY) + (parts[i].aY/* * dt * dt*/);
		parts[i].pY += (parts[i].pY - parts[i].oY) + (parts[i].aY/* * dt * dt*/);
		parts[i].oX = tempX;
		parts[i].oY = tempY;
	}
}

void eHero::accForces()
{
	for(int i = 0; i <= div; i++)
	{
		parts[i].aY = g;
	}
}

void eHero::satConstraints()
{
	for (int i = 1; i <= div; i++)
	{
		float dx = (parts[i].pX - parts[i - 1].pX);
		float dy = (parts[i].pY - parts[i - 1].pY);
		float d = LowLevel::SquareRootFloat((dx * dx) + (dy * dy));
		float diff = (d - len)*0.5;
		float constx = (dx / d) * diff;
		float consty = (dy / d) * diff;
		parts[i].pX -= constx;
		parts[i].pY -= consty;
		parts[i - 1].pX += constx;
		parts[i - 1].pY += consty;
	}
}

void eHero::checkColl()
{
	if(worldMan->FullPhase(this, (int)parts[0].pX, (int)parts[0].pY) || worldMan->FullPhase(this, (int)parts[0].pX, (int)parts[0].pY+7) )
	{
		parts[0].pX = parts[0].oX;
		parts[0].pY = parts[0].oY;
	}
	for (int i = 1; i < div; i++)
	{
		if(worldMan->FullPhase(this, (int)parts[i].pX, (int)parts[i].pY) )
		{
			parts[i].pX = parts[i].oX;
			parts[i].pY = parts[i].oY;
		}
	}
}
