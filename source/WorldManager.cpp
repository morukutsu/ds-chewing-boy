/* --------------------------- */
/* Star Plot            */
/* --------------------------- */

//Includes
#include "WorldManager.h"
#include "System/RessourceManager.h"

bool contactFilter[8][8] = {{1,1,1,1,1,1,0,0},
							{1,0,0,1,1,1,0,0},
							{1,0,0,0,1,1,0,0},
							{1,1,0,0,0,1,0,0},
							{1,1,1,0,0,1,0,0},
							{1,1,1,1,1,1,0,0},
							{1,1,1,1,1,1,0,0},
							{1,1,1,1,1,1,0,0} };

//Initialisation du worldmanager
void WorldManagerOffline::Init()
{	
	LowLevel::ToggleRearPlane(true);
	LowLevel::ParticleGenerator::getInstance()->worldMan = this;
	
	sHitBoxRect tmpaabbscreen = {0, 0, 256, 192};
	aabbScreen = tmpaabbscreen;
	LowLevel::LoadBackgroundEFSSub("efs:/data/backgrounds/bottom_screen.img.bin", "efs:/data/backgrounds/bottom_screen.pal.bin");
	
	//LoadMap("efs:/essai.tmx");

	//Test sprites 2D
	/*spr.Assign(System::CRessourceManager::getInstance()->LoadImage2D("efs:/ball.pcx") );
	spr.MoveTo(100, 100);*/
	
	isAnyNonSensorOnScreen = false;
	mNonSensorOnScreenCount = 0;

	mSkyTransActivated = true;
	mSkyTransTime = 0;
	mCurrentSkyColor = mCurrentSkyColorOrig = RGB15(0, 0, 15);
	mSkyTransfTime = 0.0f;
	mSkyTransAlpha = 31;
	

	oldScrollX = oldScrollY = 0;
	mCameraMode = CAMERA_FOLLOW;

	mGravity = 32;
	mGravityMax = 512; // 768
	
	fadeAlpha = 1;
	activateFadeBlack = false;

	mGuiGem.Assign(System::CRessourceManager::getInstance()->LoadImage("efs:/data/sprites/spr_gemme_gui.png", UL_IN_VRAM, UL_PF_PAL4) );
	mGuiBlob.Assign(System::CRessourceManager::getInstance()->LoadImage("efs:/data/sprites/jello_sheet_gui.png", UL_IN_VRAM, UL_PF_PAL4) );
	
	
}

//Nettoyage du worldmanager
void WorldManagerOffline::Cleanup()
{
	ResetBeforeReload();
	mEntities.clear();
}

//Resets à faire avant de recharger un niveau
void WorldManagerOffline::ResetBeforeReload()
{
	//Effets
	/*mBlackBarH = 0;
	mBlackBarActive = false;
	mBlackBarGrowing = false;
	mBlackBarReducing = false;
	mGuiLevelScale = mGuiLevelX = mGuiLevelAlpha = 0;
	mLevelStartingTime = 0;*/

	//Flush du moteur physique
	for(int k = 0; k < 128; k++)
		mBodies[k].isFree = true, mBodies[k].toDelete = false, mBodies[k].who = NULL;
	mNextBody = 0;
	mNextFreeBody = -1;
	mLevelLoose = false;
}

//Chargement d'un niveau
void WorldManagerOffline::LoadMap(char* filename)
{
	//Renseignement de l'instance du worldmanager
	mMap.worldMan = this;
	mFrameCounter = 0;
	plop = 0;
	mLevelGemmesCount = mLevelGemmesMax = 0;
	mLifes = 5;
	openScreenX = -31*2;
	scrollFadeActivated = true;
	mLevelFinished = false;
	mFinishTimer = true;
	mTimeUp = false;
	mLevelLoose = false;

	//Resets
	ResetBeforeReload();
	
	//Loading tileset
	mMap.Load("");
	mBgStars.Assign(System::CRessourceManager::getInstance()->LoadImage("efs:/data/backgrounds/bg_stars.png", UL_IN_VRAM, UL_PF_PAL2));
	mBgStars.MoveTo(0, 0);

	//Chargement des nuages paralalx
	mSkyCloudsColor = RGB15(0, 0, 15);
	/*for(int k = 0; k < 4; k++)
	{
		mParallaxClouds[k].w = 64;
		mParallaxClouds[k].spd = 384*2;
		mParallaxClouds[k].mSpr.Tint(mSkyCloudsColor);
		mParallaxClouds[k].mSpr.Assign(System::CRessourceManager::getInstance()->LoadImage("efs:/data/backgrounds/bg_cloud_gros.png", UL_IN_VRAM, UL_PF_PAL2));
	}
	for(int k = 4; k < 10; k++)
	{
		mParallaxClouds[k].w = 32; 
		mParallaxClouds[k].spd = 384*2/2;
		mParallaxClouds[k].mSpr.Tint(mSkyCloudsColor);
		mParallaxClouds[k].mSpr.Assign(System::CRessourceManager::getInstance()->LoadImage("efs:/data/backgrounds/bg_cloud_moyen.png", UL_IN_VRAM, UL_PF_PAL2));
	}
	for(int k = 10; k < 16; k++)
	{
		mParallaxClouds[k].w = 16;
		mParallaxClouds[k].spd = 384*2/4;
		mParallaxClouds[k].mSpr.Tint(mSkyCloudsColor);
		mParallaxClouds[k].mSpr.Assign(System::CRessourceManager::getInstance()->LoadImage("efs:/data/backgrounds/bg_cloud_petit.png", UL_IN_VRAM, UL_PF_PAL2));
	}
	mParallaxClouds[0].x = 0<<8;    mParallaxClouds[0].y = 70<<8;
	mParallaxClouds[1].x = 92<<8;   mParallaxClouds[1].y = 140<<8;
	mParallaxClouds[2].x = 187<<8;  mParallaxClouds[2].y = 92<<8;
	mParallaxClouds[3].x = 227<<8;  mParallaxClouds[3].y = 163<<8;

	mParallaxClouds[4].x = 10<<8;    mParallaxClouds[4].y = 10<<8;
	mParallaxClouds[5].x = 85<<8;   mParallaxClouds[5].y = 140<<8;
	mParallaxClouds[6].x = 130<<8;  mParallaxClouds[6].y = 53<<8;
	mParallaxClouds[7].x = 32<<8;  mParallaxClouds[7].y = 80<<8;
	mParallaxClouds[8].x = 227<<8;  mParallaxClouds[8].y = 172<<8;
	mParallaxClouds[9].x = 196<<8;  mParallaxClouds[9].y = 58<<8;

	mParallaxClouds[10].x = 0<<8;    mParallaxClouds[10].y = 0<<8;
	mParallaxClouds[11].x = 80<<8;   mParallaxClouds[11].y = 50<<8;
	mParallaxClouds[12].x = 120<<8;  mParallaxClouds[12].y = 15<<8;
	mParallaxClouds[13].x = 248<<8;  mParallaxClouds[13].y = 140<<8;
	mParallaxClouds[14].x = 210<<8;  mParallaxClouds[14].y = 92<<8;
	mParallaxClouds[15].x = 157<<8;  mParallaxClouds[15].y = 75<<8;*/

	//Ajout camera + hero
	//mEntities.push_back(&mCam);

	//Parsing du fichier XML
	TiXmlDocument doc(filename);
	doc.LoadFile();
	TiXmlElement* root = doc.FirstChildElement("map");
	
	
	//Lecture infos importantes
	root->Attribute("width", &mMap.mMapWidth);
	root->Attribute("height", &mMap.mMapHeight);
	mMap.mMapWidthHeight = mMap.mMapWidth*mMap.mMapHeight;
	
	//Chargemment des entités
	mHero = new eHero();
	mHero->Load();
	mEntities.push_back(mHero);

	//Lecture données layer
	TiXmlElement* elem = root->FirstChildElement("layer");
	char eName[32], layName[32];
	while(elem)
	{
		strcpy(eName, elem->Value() );
		if(strcmp(eName, "layer") != 0)
			break;

		strcpy(layName, elem->Attribute("name"));

		//Chargement du layer base64
		if(strcmp(layName, "Calque 1") == 0)
		{
			TiXmlElement* elem2 = elem->FirstChildElement("data");
			const char* b64txt = elem2->GetText();
	
			//Chargemment de la map (graphique)
			mMap.LoadLayer((const unsigned char*)b64txt, 0);
		}
		else if(strcmp(layName, "Collision") == 0)
		{
			TiXmlElement* elem2 = elem->FirstChildElement("data");
			const char* b64txt = elem2->GetText();
	
			//Chargemment de la map (graphique)
			mMap.LoadLayer((const unsigned char*)b64txt, 3);
		}

		elem = elem->NextSiblingElement();
	}
	
	//Chargement des pentes
	LoadMapSlopes("efs:/data/chipsets/ch_stand.slopes");
	
	LoadMapEntities(doc);

	//On déplace le héros à la position de départ
	mHero->x = mHero->lastJumpPosX = itofx(mEntryX);
	mHero->y = mHero->lastJumpPosY = itofx(mEntryY);
	
	LowLevel::SoundManager::getInstance()->StopMusic();
	LowLevel::SoundManager::getInstance()->PlayMusic("efs:/data/music/Beach-ima.wav", 1);

	//Test
	//bs::Emitter* emit = BulletManager::getInstance()->machine.createEmitter("Abstract", 128<<8, 96<<8, 0, NULL);
}

//Interprete le script de tout un niveau
void WorldManagerOffline::LoadMapEntities(TiXmlDocument &doc)
{
	//Parsing du fichier XML
	//TiXmlDocument doc(filename);
	//doc.LoadFile();
	TiXmlElement* root = doc.FirstChildElement("map");

	//Lecture données layer
	TiXmlElement* elem = root->FirstChildElement("objectgroup")->FirstChildElement();
	char entName[32];
	int px, py, pw, ph;
	while(elem)
	{
		//Récupération du nom de l'entité
		strcpy(entName, elem->Attribute("name"));
		//ulDebug("%s\n", elem->Attribute("name"));
		elem->Attribute("x", &px);
		elem->Attribute("y", &py);
		elem->Attribute("width", &pw);
		elem->Attribute("height", &ph);

		LoadEntity(entName, px, py, pw, ph, 0, 0, -1);

		elem = elem->NextSiblingElement();
	}
}

//Interprete le script de tout un niveau
void WorldManagerOffline::LoadMapScript(char* filename)
{
	
}

//Chargement des pentes
void WorldManagerOffline::LoadMapSlopes(char* filename)
{
	//On charge la liste des pentes
	FILE* fichier = fopen(filename, "rb");
	
	mNumPentes = 0;
	fread(&mNumPentes, sizeof(int), 1, fichier);
	for(int k = 0; k < mNumPentes; k++)
	{
		int slope;
		fread(&slope, sizeof(int), 1, fichier);
		
		mPentes[k].v = slope;
		mPentesTable[GetTileX(slope)][GetTileY(slope)] = k;
	}
	fclose(fichier);

	//On calcule la hauteur de chaque pixel
	for(int k = 0; k < mNumPentes; k++)
	{
		int u0 = GetTileX(mPentes[k].v)<<4;
		int v0 = GetTileY(mPentes[k].v)<<4;

		//On sauvegarde les hauteurs
		for(u8 x = 0; x < 16; x++)
		{
			//Pour chaque colonne
			for(u8 y = 0; y < 16; y++)
			{
		   		//On récupère le pixel (on suppose tjrs une image 8 bits et des tiles de 16²)
		   		void *pPixel = ulGetImageLineAddr(mMap.mChipsetColl->img, v0+y);
		   		u8 pix = ((u8*)pPixel)[u0+x];
		   		//Si ce pixel est bleu on enregistre les positions
		   		if(pix == 3 || pix == 4)
		   		{
		   			mPentes[k].hauteur[x] = y;
		   			//On arrête la recherche
		   			break;
		   		}  
		   }
		}
	}
}

//Rechargement après une vie perdue
void WorldManagerOffline::Reload()
{
	
}

//Rechargement après une vie perdue
void WorldManagerOffline::FreeWorld()
{
	
}

//On ajoute un evènement
void WorldManagerOffline::PushEvent(int id, int eventType)
{

}

//Vidage de la map
void WorldManagerOffline::FreeMap()
{
    
}

//Regeneration du monde (après qu'une vie soit perdue)
void WorldManagerOffline::RegenWorld()
{

}

//Tremblement
void WorldManagerOffline::RumbleCamera(int frames)
{
	
}


//Mise a jour du monde
void WorldManagerOffline::Update()
{
	LowLevel::SoundManager::getInstance()->Handle();
	oldScrollX = scrollX;
	oldScrollY = scrollY;
	scrollX = -mCam.x;
	scrollY = -mCam.y;
	
	//Déplacement écran ouverture
	if(scrollFadeActivated)
	{
		if(openScreenX > 256+31*2)
			scrollFadeActivated = false;
		openScreenX += 4;
	}

	if(mLevelFinished || mLevelLoose)
	{
		mHero->allowMove = false;
		mFinishTimer++;
		if(mFinishTimer == 90)
			activateFadeBlack = true;
		if(fadeAlpha > 31)
			fadeAlpha = 31;
	}

	//Debug
	/*if(ul_keys.pressed.start)
	{
		mHero->x = itofx(mStartPosX);
		mHero->y = itofx(mStartPosY);
	}

	*/

	if(mCameraMode == CAMERA_FOLLOW)
	{
		//Calcul du scrolling
		oldScrollX = scrollX;
		oldScrollY = scrollY;
		scrollX = itofx(128) - mHero->x - itofx(16);
		scrollY = itofx(96) - mHero->y - itofx(16);
		if(scrollX >= 0)
			scrollX = 0;
		if(scrollY >= 0)
			scrollY = 0;
		if(scrollX <= -itofx(mMap.mMapWidth*16) + itofx(256) )
			scrollX = -itofx(mMap.mMapWidth*16) + itofx(256);
		if(scrollY <= -itofx(mMap.mMapHeight*16) + itofx(192) )
			scrollY = -itofx(mMap.mMapHeight*16) + itofx(192);
	}
	
	//Collision
	HandleBodyCollision();

	//Update des entités
	for(mEntitiesItor = mEntities.begin(); mEntitiesItor != mEntities.end (); )
	{
		if((*mEntitiesItor)->toDelete)
		{
			delete (*mEntitiesItor);
			mEntities.erase(mEntitiesItor++);
		}
		else
		{
			(*mEntitiesItor)->oldvx = (*mEntitiesItor)->vx;
			(*mEntitiesItor)->oldvy = (*mEntitiesItor)->vy;
			
			//Application de la gravité aux objets
			if((*mEntitiesItor)->density > 0 && (*mEntitiesItor)->real && (*mEntitiesItor)->actif)
			{
				(*mEntitiesItor)->vy += Mulfx(mGravity, (*mEntitiesItor)->density);
				if((*mEntitiesItor)->vy >= Mulfx(mGravityMax, (*mEntitiesItor)->density))
					(*mEntitiesItor)->vy = Mulfx(mGravityMax, (*mEntitiesItor)->density);
			}

			//Mise à jour
			(*mEntitiesItor)->Update();

			if((*mEntitiesItor)->real && (*mEntitiesItor)->actif)
				ProcessEntityPosition((*mEntitiesItor));

			++mEntitiesItor;
		}
	}
	
	//Update map
	//mMap.Cull();
	
	//MAJ des effets
	//Bandes noires
	/*if(mBlackBarActive)
	{
		if(mBlackBarGrowing)
		{
			mBlackBarH++;
			if(mBlackBarH >= 10)
				mBlackBarGrowing = false;
		}
		if(mBlackBarReducing)
		{
			mBlackBarH--;
			if(mBlackBarH <= 0)
			{
				mBlackBarReducing = false;
				mBlackBarActive = false;
			}
		}
	}*/
	
	//Fin de niveau
	if(mFrameCounter >= mLevelTime)
	{
		mLevelLoose = true;
		mTimeUp = true;
		
	}

	//Update framecounter pour le scripting
	if(!mLevelFinished && !mLevelLoose)
	{
		//ulDebug("%d\n", mFrameCounter);
		mFrameCounter++;
	}
	
	//ulDebug("f : %d\n", mFrameCounter);
	//mFrameCounter++;
	//plop++;
	//ulDebug(" p :%d\n", plop);

	//ulDebug("fin : %d , loose : %d\n", mLevelFinished, mLevelLoose);
}

//Récupération d'id pour les script
CEntity* WorldManagerOffline::GetElementById(int id)
{
	if(id == 0)
	{
		return &mCam;
	}
	else
	{
		return NULL;
	}
}

//BG evolutif
void WorldManagerOffline::ShowSkyBG(int color)
{
	int c1 = RGB15(
		ulMin(ulGetColorRed(color)+5, 31), 
		ulMin(ulGetColorGreen(color)+5, 31), 
		ulMin(ulGetColorBlue(color)+5, 31) ); 
	ulDrawGradientRect(0, 0, 256, 192, color, color, c1, c1);
	//ulDrawFillRect(0, 64, 256, 92, c1);
}

//Affichage du monde
void WorldManagerOffline::Draw()
{
	//Affichage du background sky
	if(mSkyTransActivated)
	{
		//Passage vers le jour
		if(mSkyTransTime >= 600)
		{
			mSkyTransAlpha = LERP_Simplef(mSkyTransfTime, 0.0f, 1.0f, 31, 0);

			mSkyTransfTime += 0.0025f;
			if(mSkyTransfTime >= 1.0f)
				mSkyTransfTime = 1.0f;
			
			mSkyCloudsColor = RGB15(
				(int)LERP_Simplef(mSkyTransfTime, 0.0f, 1.0f, 0, 31), 
				(int)LERP_Simplef(mSkyTransfTime, 0.0f, 1.0f, 0, 31), 
				(int)LERP_Simplef(mSkyTransfTime, 0.0f, 1.0f, 15, 31) ); 
			/*for(int k = 0; k < 16; k++)
				mParallaxClouds[k].mSpr.Tint(mSkyCloudsColor);*/
		}

		//Fin du passage
		if(mSkyTransTime == 1200)
			mSkyTransActivated = false;
		mSkyTransTime++;
	}
	ShowSkyBG(RGB15(0, 20, 29));
	if(mSkyTransAlpha > 0)
	{
		ulSetAlpha(UL_FX_ALPHA, mSkyTransAlpha, 1);
		ShowSkyBG(mCurrentSkyColor);
		ulSetAlpha(UL_FX_DEFAULT, 0, 0);
		
		mBgStars.Alpha(mSkyTransAlpha, 2);
		mBgStars.Draw();
	}

	//Effets nuages (Fond)
	/*for(int k = 10; k < 16; k++)
	{
		mParallaxClouds[k].mSpr.MoveTo(mParallaxClouds[k].x>>8, mParallaxClouds[k].y>>8);
		mParallaxClouds[k].mSpr.Draw();
		mParallaxClouds[k].x -= mParallaxClouds[k].spd;
		if(mParallaxClouds[k].x < -mParallaxClouds[k].w<<8)
			mParallaxClouds[k].x = itofx(256+mParallaxClouds[k].w);
		
	}
	for(int k = 4; k < 10; k++)
	{
		mParallaxClouds[k].mSpr.MoveTo(mParallaxClouds[k].x>>8, mParallaxClouds[k].y>>8);
		mParallaxClouds[k].mSpr.Draw();
		mParallaxClouds[k].x -= mParallaxClouds[k].spd;
		if(mParallaxClouds[k].x < -mParallaxClouds[k].w<<8)
			mParallaxClouds[k].x = itofx(256+mParallaxClouds[k].w);
	}*/

	
	//Ajustement du scrolling
	//mFixedEntity = mHero;
	//mCameraMode = CAMERA_FOLLOW;
	
	mMap.MoveTo(scrollX>>8, scrollY>>8);
	//mMap.MoveTo(0, 0);
	mMap.Cull();
	
	//Affichage des entités
	for(mEntitiesItor = mEntities.begin(); mEntitiesItor != mEntities.end (); ++mEntitiesItor)
		(*mEntitiesItor)->Draw(0);
	
	mMap.DrawMap(0);

	for(mEntitiesItor = mEntities.begin(); mEntitiesItor != mEntities.end (); ++mEntitiesItor)
		(*mEntitiesItor)->Draw(1);

	LowLevel::ParticleGenerator::getInstance()->DrawGens();
	
	//Affichage des bullets par dessus les entités (et sous les nuages)

	//Effets nuages (Devant)
	/*for(int k = 0; k < 4; k++)
	{
		mParallaxClouds[k].mSpr.MoveTo(mParallaxClouds[k].x>>8, mParallaxClouds[k].y>>8);
		mParallaxClouds[k].mSpr.Draw();
		mParallaxClouds[k].x -= mParallaxClouds[k].spd;
		if(mParallaxClouds[k].x < -(mParallaxClouds[k].w<<8))
			mParallaxClouds[k].x = itofx(256+mParallaxClouds[k].w);
	}*/


	//Affichage Map (layer 1 et 2)
	/*if(mWhiteScreenAlpha != 31)
	{
		mMap.DrawMap(0);
		mMap.DrawMap(1);

		for(mEntitiesItor = mEntities.begin(); mEntitiesItor != mEntities.end (); ++mEntitiesItor)
			(*mEntitiesItor)->Draw(0);
		for(mEntitiesItor = mEntities.begin(); mEntitiesItor != mEntities.end (); ++mEntitiesItor)
			(*mEntitiesItor)->Draw(1);
		LowLevel::ParticleGenerator::getInstance()->DrawGens();

		//Affichage Map (layer 3)
		mMap.DrawMap(2);
		mMap.DrawMap(3);
	}*/
	
	//Affichage des bandes noires
	/*if(mBlackBarActive)
	{
		LowLevel::drawRect(0, 0, 256, mBlackBarH, RGB15(0, 0, 0) );
		LowLevel::drawRect(0, 192-mBlackBarH, 256, 192, RGB15(0, 0, 0) );
	}*/
	
	
	/*for(int k = 0; k < 128; k++)
	{
		if(!mBodies[k].isFree)
		{
			ulSetAlpha(UL_FX_ALPHA, 15, 1);
			int color;
			switch(mBodies[k].userdata)
			{
				case BODY_HERO: color = RGB15(31, 0, 31); break;
				case BODY_ENNEMY: color = RGB15(0, 0, 31); break;
				case BODY_HERO_HIT: color = RGB15(31, 0, 0); break;
				default : color = RGB15(10, 10, 10); break;
			};

			LowLevel::drawRect(mBodies[k].x + fxtoi(scrollX) + mBodies[k].aabb->x1, 
				mBodies[k].y + fxtoi(scrollY) + mBodies[k].aabb->y1, 
				mBodies[k].x + mBodies[k].aabb->x2 + fxtoi(scrollX), 
				mBodies[k].y + mBodies[k].aabb->y2 + fxtoi(scrollY), color );
			ulSetAlpha(UL_FX_DEFAULT, 0, 0);
		}
	}*/
	
	//Fondu
	if(scrollFadeActivated)
	{
		for(int k = 1; k < 31; k++)
		{
			LowLevel::drawRect(openScreenX+k*2, 0, openScreenX+k*2+2, 192, 0, k);
		};
		LowLevel::drawRect(openScreenX+31*2, 0, openScreenX+31*2+256, 192, 0, 31);
	}
	
	if(activateFadeBlack)
	{
		LowLevel::drawRect(0, 0, 256, 192, 0, fadeAlpha);
		fadeAlpha += 1;
		/*if(fadeAlpha == 32)
		{
			//Création du chemin
			char levelFilename[256];
			sprintf(levelFilename, "efs:/data/maps/level%d.tmx", mSelectedLevel);

			//Démarrage de la level state avec le chemin
			game->ChangeState(LevelState::Instance() );
			LevelState::Instance()->worldMan->LoadMap(levelFilename);
		
		}*/
	}

	if(mLevelFinished)
	{
		LowLevel::drawString(256/2-LowLevel::getStringWidth("Level Finished !")/2, 192/2-8, "Level Finished", 32767);
		if(mLevelGemmesCount == mLevelGemmesMax)
			LowLevel::drawString(256/2-LowLevel::getStringWidth("Perfect")/2, 192/2+10, "Perfect", RGB15(0, 31, 31) );
	}

	if(mLevelLoose)
	{
		LowLevel::drawString(256/2-LowLevel::getStringWidth("Game Over !")/2, 192/2-8, "Game Over !", 32767);
		if(mTimeUp)
			LowLevel::drawString(256/2-LowLevel::getStringWidth("Time up")/2, 192/2+10, "Time up", RGB15(0, 31, 31));
	}


	//Affichage du GUI
	//Barre de pouvoir du héros
	ulSetTextColor(RGB15(31, 31, 31));
	if(mHero->currentBall == 0)
		ulPrintf_xy(5, 5, ".basic ball");
	else if(mHero->currentBall == 1)
		ulPrintf_xy(5, 5, ".stick ball");
	else if(mHero->currentBall == 2)
		ulPrintf_xy(5, 5, ".fire ball");
	else if(mHero->currentBall == 3)
		ulPrintf_xy(5, 5, ".verlet ball");
	
	ulDrawFillRect(84, 5, 186, 12, 0);
	ulDrawGradientRect(85, 6, 85 + (mHero->powerBall>>8), 11, RGB15(31, 5, 5), RGB15(31, 5, 5), RGB15(31, 31, 5), RGB15(31, 31, 5) );
	
	//Affichage des icones
	ulSetTextColor(RGB15(31, 31, 31));
	mGuiGem.MoveTo(5, 192-16);
	mGuiGem.Draw();
	if(mLevelGemmesCount >= mLevelGemmesMax/2)
		ulSetTextColor(RGB15(6, 6, 31));
	ulPrintf_xy(5+18, 192-16+5, "x %d/%d", mLevelGemmesCount, mLevelGemmesMax);
	ulSetTextColor(RGB15(31, 31, 31));
	mGuiBlob.MoveTo(60+5, 192-16-1);
	mGuiBlob.Draw();
	int showVal = mLifes >= 0 ? mLifes : 0;
	ulPrintf_xy(60+5+18, 192-16+5, "x %d", showVal);
	
	//ulDebug("diff : %d - ft : %d - lt : %d\n", mLevelTime-mFrameCounter, mFrameCounter, mLevelTime);

	//Calcul du temps du niveau
	int minutes, secondes, centiemes;
	centiemes = (mLevelTime-mFrameCounter)%60;
	secondes = (((mLevelTime-mFrameCounter)-centiemes)/60)%60;
	minutes = (((mLevelTime-mFrameCounter)-centiemes)/60)/60;
	
	char time[32] = "";
	sprintf(time, "Time : %02d:%02d:%02d", minutes, secondes, (int)((float)centiemes*100.0f/60.0f));
	ulPrintf_xy(60+5+18+80, 192-16+5, time);

	//Debug
	ulSetTextColor(RGB15(31, 0, 0));
	//ulPrintf_xy(80, 10, "Frame: %d", mFrameCounter);

}

//Calcule la nouvelle position de l'entité
void WorldManagerOffline::ProcessEntityPosition(CEntity* ent)
{
	//Variables & Points de test
	bool groundFound = false;
	bool slopeFound = false;
	bool wallFound = false;
	bool plafondFound = false;
	
	if(!ent->ignoreCollision)
	{
		//Maj états entité
		ent->mFallLeftSignal = ent->mFallRightSignal = false;
		ent->mWallAtLeft = ent->mWallAtRight = false;
		ent->isOnTheFloor = false;

		int testx = fxtoi(ent->x)+ent->mHitPoints.pBottomX;
		int testy = fxtoi(ent->y)+ent->mHitPoints.pBottomY-1;
		
		sPoint mTestPoints[14];
		mTestPoints[0].x = testx, mTestPoints[0].y = testy;
		mTestPoints[1].x = testx, mTestPoints[1].y = testy-1;
		mTestPoints[2].x = testx-1, mTestPoints[2].y = testy;
		mTestPoints[3].x = testx+1, mTestPoints[3].y = testy;
		mTestPoints[4].x = testx, mTestPoints[4].y = testy+4;
		mTestPoints[5].x = testx, mTestPoints[5].y = testy+4;

		mTestPoints[6].x = fxtoi(ent->x)+ent->mHitPoints.pTopX1, mTestPoints[6].y = fxtoi(ent->y)+ent->mHitPoints.pTopY1;
		mTestPoints[7].x = fxtoi(ent->x)+ent->mHitPoints.pTopX2, mTestPoints[7].y = fxtoi(ent->y)+ent->mHitPoints.pTopY2;

		mTestPoints[8].x = fxtoi(ent->x)+ent->mHitPoints.pTopSideX+1;
		mTestPoints[8].y = fxtoi(ent->y)+ent->mHitPoints.pTopSideY;
		
		mTestPoints[9].x = fxtoi(ent->x)+ent->mHitPoints.pTopSideXr;
		mTestPoints[9].y = fxtoi(ent->y)+ent->mHitPoints.pTopSideY;

		mTestPoints[10].x = fxtoi(ent->x)+ent->mHitPoints.pBottomSideX+1;
		mTestPoints[10].y = fxtoi(ent->y)+ent->mHitPoints.pBottomSideY;

		mTestPoints[11].x = fxtoi(ent->x)+ent->mHitPoints.pBottomSideXr;
		mTestPoints[11].y = fxtoi(ent->y)+ent->mHitPoints.pBottomSideY;

		mTestPoints[12].x = fxtoi(ent->x)+ent->mHitPoints.pFallX1;
		mTestPoints[12].y = fxtoi(ent->y)+ent->mHitPoints.pFallY1;

		mTestPoints[13].x = fxtoi(ent->x)+ent->mHitPoints.pFallX2;
		mTestPoints[13].y = fxtoi(ent->y)+ent->mHitPoints.pFallY2;


		//Routines de collision avec des objets solides

		//Collisions avec la map

		//1 : Axe Y
		ent->oldx = ent->x;
		ent->oldy = ent->y;
		
		if(ent->vy > 0)
		{
			//Test de collision avec les pentes
			for(int k = 0; k <= 4; k++)
			{
				if(k != 4)
				{
					slopeFound = SlopePhase(ent, mTestPoints[k].x, mTestPoints[k].y + fxtoi(ent->vy) );
				}
				else
				{
					if(ulAbs(ent->vx) >= 384)
						slopeFound = SlopePhase(ent, mTestPoints[k].x, mTestPoints[k].y + fxtoi(ent->vy) );
				}
				if(slopeFound == true)
				{
					groundFound = true;
					break;
				}
			}
		}

		if(slopeFound == false)
		{
			if(ent->vy > 0)
			{
				//Test de collision avec le sol
				bool r1 = FlatPhase(ent, mTestPoints[12].x, mTestPoints[12].y + fxtoi(ent->vy) );
				bool r2 = FlatPhase(ent, mTestPoints[13].x, mTestPoints[13].y + fxtoi(ent->vy) );
				if(r1 || r2)
					groundFound = true;

				//Renseignements signaux chute
				if(r1 && !r2)
				{
					//On regarde si il n'y a pas une pente juste en bas
					if(!SlopePhaseCheck(ent, mTestPoints[13].x, mTestPoints[13].y + 2) &&
						!SlopePhaseCheck(ent, mTestPoints[13].x, mTestPoints[13].y + 3) )
						ent->mFallLeftSignal = true;
				}
				if(!r1 && r2)
				{
					if(!SlopePhaseCheck(ent, mTestPoints[12].x, mTestPoints[12].y + 2) &&
						!SlopePhaseCheck(ent, mTestPoints[12].x, mTestPoints[12].y + 3) )
						ent->mFallRightSignal = true;
				}
				
			}
		}
		
		//On applique les différent états si un sol à été trouvé
		if(groundFound == true)
		{
			//Pas de vitesse verticale
			ent->vy = 0;
			ent->isOnTheFloor = true;
		}

		//Collisions avec le plafond
		if(ent->vy < 0)
		{
			bool r1 = PlafondPhase(ent, mTestPoints[6].x, mTestPoints[6].y + fxtoi(ent->vy) );
			bool r2 = PlafondPhase(ent, mTestPoints[7].x, mTestPoints[7].y + fxtoi(ent->vy) );
			if(r1 || r2)
				plafondFound = true;
		}
		
		if(plafondFound)
		{
			ent->vy = 0;
		}

		//Vitesse maximale
		/*if(slopeFound)
		{
			//On essaie de savoir si il descend la pente ou l'inverse

			//On règle la vitesse du personnage
			if(ent->isInWater)
				ent->mMaxSpeedX = ent->mMaxSpeedXSlope>>1;
			else
				ent->mMaxSpeedX = ent->mMaxSpeedXSlope;
		}
		else if(groundFound)
		{
			//On règle la vitesse du personnage
			if(ent->isInWater)
				ent->mMaxSpeedX = ent->mMaxSpeedXRun>>1;
			else
				ent->mMaxSpeedX = ent->mMaxSpeedXRun;
		}*/
		if(ent->isInWater)
			ent->mMaxSpeedX = ent->mMaxSpeedXRun>>1;
		else
			ent->mMaxSpeedX = ent->mMaxSpeedXRun;

		//2 : Axe X
		//On teste les murs sur les côtés
		for(int k = 8; k <= 11; k++)
		{
			//On teste le 2ème pt seulement si on tombe ou on monte
			if(ent->vy == 0)
			{
				if(k == 10 || k == 11)
					break;
			}

			wallFound = WallPhase(ent, mTestPoints[k].x + fxtoi(ent->vx), mTestPoints[k].y );
			if(wallFound == true)
			{
				//Info sur la position du mur
				if(k == 8 || k == 10)
					ent->mWallAtRight = true;
				if(k == 9 || k == 11)
					ent->mWallAtLeft = true;
				break;
			}
		}

		if(wallFound)
		{
			ent->vx = 0;
		}

		//Limites sortie niveau
		if(ent->x + ent->vx < 0)
			ent->vx = 0;
		if(ent->x + ent->vx + itofx(ent->w) > itofx(mMap.mMapWidth<<4))
			ent->vx = 0;
	}

	//On applique les déplacements éventuels
	ent->y += ent->vy;
	ent->x += ent->vx;
}

bool WorldManagerOffline::SlopePhase(CEntity* ent, int testx, int testy)
{
	if(mMap.GetMapPixel(testx, testy) == 3 || mMap.GetMapPixel(testx, testy) == 4)
	{
		//On ajuste la hauteur du héros
		int mx = (testx)>>4;
		int my = (testy)>>4;

		//On cherche sur quel tile se trouve le héros
		u16 block = sMapArray[3 * (mMap.mMapWidth*mMap.mMapHeight) + (mx * mMap.mMapHeight) + my];
		int k = mPentesTable[GetTileX(block)][GetTileY(block)];
		
		//On cherche si le slope est mirrored en X 
		//(faire le mirror d'un slope en Y est... bête... vous marchez au plafond vous ?)
		//bool mirrorX = GetTileMirrorX(block);

		//On récup la position sur le tile
		u8 diff = testx%16;
		int maxy = 0;
		maxy = mPentes[k].hauteur[diff];

		//On calcule les coordonnées en haut du tile en y 
		int tpy = (testy>>4)<<4;

		//On place le héros à la bonne hauteur
		ent->y = itofx(tpy - ent->mHitPoints.pBottomY + maxy + 1);

		return true;
	}
	else
	{
		return false;
	}
}

bool WorldManagerOffline::SlopePhaseCheck(CEntity* ent, int testx, int testy)
{
	int pix = mMap.GetMapPixel(testx, testy);
	if(pix == 3 || pix == 4)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool WorldManagerOffline::StickPhase(CEntity* ent, int testx, int testy)
{
	int pix = mMap.GetMapPixel(testx, testy);
	if(pix == 4 || pix == 5)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool WorldManagerOffline::FlatPhase(CEntity* ent, int testx, int testy)
{
	int pix = mMap.GetMapPixel(testx, testy);
	if(pix == 2 || pix == 5)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool WorldManagerOffline::FullPhase(CEntity* ent, int testx, int testy)
{
	int pix = mMap.GetMapPixel(testx, testy);
	if(pix == 2 || pix == 3 || pix == 4 || pix == 5)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool WorldManagerOffline::WallPhase(CEntity* ent, int testx, int testy)
{
	int pix = mMap.GetMapPixel(testx, testy);
	if(pix == 2)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool WorldManagerOffline::PlafondPhase(CEntity* ent, int testx, int testy)
{
	if(mMap.GetMapPixel(testx, testy) == 2)
	{
		return true;
	}
	else
	{
		return false;
	}
}

int WorldManagerOffline::AddBody(int x, int y, CEntity* ent, int userdata)
{
	//On crée un body
	CBody tmp;
	tmp.shpx = x;
	tmp.shpy = y;
	tmp.who = ent;
	tmp.isFree = false;
	tmp.isActive = true;
	tmp.toDelete = false;
	//tmp.isSleeping = false;
	tmp.userdata = userdata;
	
	//On ajoute le body
	if(mNextFreeBody != -1)
	{
		mBodies[mNextFreeBody] = tmp;
		int saved = mNextFreeBody;
		mNextFreeBody = -1;
		return saved;
	}
	else
	{
		mBodies[mNextBody] = tmp;
		mNextBody++;
		return mNextBody-1;
	}
}

void WorldManagerOffline::HandleBodyCollision()
{
	//Decl de variables
	bool result = false;
	isAnyNonSensorOnScreen = false;
	mNonSensorOnScreenCount = 0;

	//Flush
	for(int k = 0; k < 128; k++)
	{
		//Destroy
		if(!mBodies[k].isFree)
		{
			if(mBodies[k].toDelete)
			{
				mBodies[k].isFree = true;
				mNextFreeBody = k;
			}
			else
			{
				mBodies[k].collide = false;
				mBodies[k].isOnScreen = false;
				mBodies[k].bodyContactCount = 0;
				if(mBodies[k].who != NULL)
				{
					mBodies[k].x = mBodies[k].shpx + fxtoi(mBodies[k].who->x);
					mBodies[k].y = mBodies[k].shpy + fxtoi(mBodies[k].who->y);
				}
				else
				{
					mBodies[k].x = mBodies[k].shpx;
					mBodies[k].y = mBodies[k].shpy;
				}
			}
		}
	}

	//Parcours des bodies no 1
	for(int k = 0; k < 128-1; k++)
	{
		//On regarde si le body est créé/actif et exclusion si hors de l'écran
		if(!mBodies[k].isFree && !mBodies[k].isSleeping )
		{
			mBodies[k].isOnScreen = true;
			if(mBodies[k].isSensor == false)
			{
				mNonSensorOnScreen[mNonSensorOnScreenCount] = k;
				isAnyNonSensorOnScreen = true;
				mNonSensorOnScreenCount++;
			}
			//Parcours des bodies no 2
			for(int l = k+1; l < 128; l++)
			{
				//On regarde si le body est créé/actif
				if(!mBodies[l].isFree && !mBodies[l].isSleeping && contactFilter[mBodies[k].userdata][mBodies[l].userdata])
				{
					
					//AABB test
					if(LowLevel::AABBCollide(mBodies[k].x, mBodies[k].y, mBodies[k].aabb,
											 mBodies[l].x, mBodies[l].y, mBodies[l].aabb) )
					{
						//ulDebug("plap k:%d l:%d\n", k, l);
						//Etats
						result = true;
						mBodies[k].collide = mBodies[l].collide = true;

						//Bodies en collision
						mBodies[k].bodyContactCount++;
						mBodies[k].with[mBodies[k].bodyContactCount-1] = &mBodies[l];
						mBodies[l].bodyContactCount++;
						mBodies[l].with[mBodies[l].bodyContactCount-1] = &mBodies[k];
					}				
				}
			}
		}
	}
}

void WorldManagerOffline::LoadEntity(char* name, int x, int y, int w, int h, int p1, int p2, int id)
{
	//Gestion du chargement d'entité
	if(strcmp(name, "entry") == 0)
	{
		mEntryX = x;
		mEntryY = y;
	}
	else if(strcmp(name, "gemme") == 0)
	{
		CEntity* ent = new eGemme();
		ent->x = x;
		ent->y = y;
		ent->Load();
		mEntities.push_back(ent);
		mLevelGemmesMax++;
	}
	else if(strcmp(name, "water") == 0)
	{
		eWater* ent = new eWater();
		ent->x = itofx(x);
		ent->y = itofx(y);
		ent->w = w, ent->h = h;
		ent->def = 8;
		ent->movh = 3;
		ent->speed = 64;
		ent->Load();
		mEntities.push_back(ent);
	}
	else if(strcmp(name, "fullstar") == 0)
	{
		CEntity* ent = new eFullstar();
		ent->x = x;
		ent->y = y;
		ent->Load();
		mEntities.push_back(ent);
	}
	else if(strcmp(name, "womie") == 0)
	{
		CEntity* ent = new eWomie();
		ent->x = itofx(x);
		ent->y = itofx(y);
		ent->Load();
		mEntities.push_back(ent);
	}
	else if(strcmp(name, "peaks") == 0)
	{
		ePeaks* ent = new ePeaks();
		ent->x = x;
		ent->y = y;
		ent->orientation = p1;
		ent->Load();
		mEntities.push_back(ent);
	}
	else if(strcmp(name, "wopic") == 0)
	{
		CEntity* ent = new eWopic();
		ent->x = itofx(x);
		ent->y = itofx(y);
		ent->Load();
		mEntities.push_back(ent);
	}
	else if(strcmp(name, "dbloc") == 0)
	{
		CEntity* ent = new eDBloc();
		ent->x = x;
		ent->y = y;
		ent->Load();
		mEntities.push_back(ent);
	}
	else if(strcmp(name, "extralife") == 0)
	{
		CEntity* ent = new eExtralife();
		ent->x = x;
		ent->y = y;
		ent->Load();
		mEntities.push_back(ent);
	}

	/*if(type == ENT_TYPE_HERO)
	{
		if(genre == ENT_HERO_HERO)
		{
			
		}
	}
	if(type == ENT_TYPE_LOGIC)
	{
		if(genre == ENT_LOGIC_STARTPOS)
		{
 
		}
	}
	if(type == ENT_TYPE_OBJET)
	{

	}
	if(type == ENT_TYPE_ENNEMY)
	{
		if(genre == ENT_ENNEMY_ENNEMY)
		{
			eEnnemy * tmp = new eEnnemy();
			tmp->id = id;
			tmp->x = x;
			tmp->y = y;
			tmp->Load();
			mEntities.push_back(tmp);
		}
	}*/
}

void WorldManagerOffline::AddEntity(CEntity* entity)
{
	
}

void WorldManagerOffline::ActivateBlackBar()
{
	/*mBlackBarH = 0;
	mBlackBarActive = true;
	mBlackBarGrowing = true;
	mBlackBarReducing = false;*/
}

void WorldManagerOffline::DesactivateBlackBar()
{
	/*mBlackBarGrowing = false;
	mBlackBarReducing = true;*/
}

bool WorldManagerOffline::IsOutsideOfWorld(int x, int y )
{
	
}

void WorldManagerOffline::GeneralScale(float factor)
{

}

