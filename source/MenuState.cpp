/* --------------------------- */
/* Natura no Story             */
/* --------------------------- */

//Includes
#include "MenuState.h"
#include "System/RessourceManager.h"
#include "StateManager.h"
#include "WorldManager.h"
#include "LevelState.h"

//GFX
MenuState MenuState::m_MenuState;

void MenuState::Init()
{
	//Chargement des sprites
	setBrightness(2, -16);
	LowLevel::LoadBackgroundEFSSub("efs:/data/backgrounds/bottom_screen.img.bin", "efs:/data/backgrounds/bottom_screen.pal.bin");
	mFond.Assign(System::CRessourceManager::getInstance()->LoadImage("efs:/data/backgrounds/title.png", UL_IN_VRAM, UL_PF_PAL8) );
	mFond.MoveTo(0, 0);

	mCurrentState = mCurrentSelected = mSelectedLevel = 0;
	dontInput = false;
	grantedLevel = 4;
	
	alphaBlink = 31;
	alphaSens = false;
	
	//On vérifie si le fichier existe
	FILE * ftest = fopen("fat:/cb_scores.sc", "rb");
	if(ftest)
	{
		//Le fichier existe
		fclose(ftest);
	}
	else
	{
		//Le ficheir n'existe pas, on le crée
		ftest = fopen("fat:/cb_scores.sc", "wb+");
		if(ftest)
		{
			for(int k = 0; k < 32; k++)
			{
				levelsScores[k] = 0;
				levelsGems[k] = -1;
			}
			fwrite(levelsScores, sizeof(int), 32, ftest);
			fwrite(levelsGems, sizeof(int), 32, ftest);
			fclose(ftest);
		}
		else
		{
			//Problème de filesystem...
		}
	}
	//Chargement des str des niveaux
	//strcpy(levelsStr[0], "Getting started"); 
	levelsStr[0] = "Getting started";
	levelTimes[0] = 3*60*60;
	levelsGemsMax[0] = 20;

	//strcpy(levelsStr[1], "Level 1");
	levelsStr[1] = "Level 1";
	levelTimes[1] = 5*60*60;
	levelsGemsMax[1] = 40;

	//strcpy(levelsStr[2], "Level 2");
	levelsStr[2] = "Level 2";
	levelTimes[2] = 6*60*60;
	levelsGemsMax[2] = 20;

	//strcpy(levelsStr[3], "Level 3");
	levelsStr[3] = "Level 3";
	levelTimes[3] = 8*60*60;
	levelsGemsMax[3] = 25;

	//strcpy(levelsStr[4], "Level 4");
	levelsStr[4] = "Level 4";
	levelTimes[4] = 8*60*60;
	levelsGemsMax[4] = 24;

	//Chargement des scores des niveaux
	FILE * f = fopen("fat:/cb_scores.sc", "rb");
	if(f)
	{
		fread(levelsScores, sizeof(int), 32, f);
		fread(levelsGems, sizeof(int), 32, f);
		fclose(f);
	}

	fadeAlpha = 1;
	activateFadeBlack = false;
	
	LowLevel::SoundManager::getInstance()->StopMusic();
	LowLevel::SoundManager::getInstance()->PlayMusic("efs:/data/music/FloatingAround-ima.wav", 0);
}

void MenuState::Cleanup()
{
	LowLevel::SoundManager::getInstance()->StopMusic();
}

void MenuState::Pause()
{

}

void MenuState::Resume()
{

}

void MenuState::HandleEvents(StateManager* game)
{
    
	

}

void MenuState::Update(StateManager* game)
{

}

void MenuState::Draw(StateManager* game)
{
	//On récupère l'actions sur le menu
    LowLevel::Input *input = LowLevel::InputManager::getInstance()->GetInput();

	//Fond
	mFond.Draw();
	
	if(alphaBlink > 30)
		alphaSens = false;
	if(alphaBlink < 15)
		alphaSens = true;

	if(alphaSens)
		alphaBlink++;
	else
		alphaBlink--;
	
	
	//-- Ecran press start
	if(mCurrentState == 0)
	{
		if(!dontInput && (input->butPressedA || input->butPressedB || input->butPressedStart ||input->butPressedTouch))
			mCurrentState = 1, dontInput = true, mCurrentSelected = 0, LowLevel::SoundManager::getInstance()->PlaySoundSimple(SND_MENU);

		LowLevel::drawString(256/2-LowLevel::getStringWidth("Press a button")/2, 130, "Press a button", 32767);
	}
	//-- Menu principal
	else if(mCurrentState == 1)
	{
		//Curseur
		if(!dontInput) 
		{
			if(input->butPressedUp)
				mCurrentSelected--, LowLevel::SoundManager::getInstance()->PlaySoundSimple(SND_MENU);
			if(input->butPressedDown)
				mCurrentSelected++, LowLevel::SoundManager::getInstance()->PlaySoundSimple(SND_MENU);
			
			//Selection menus
			if(input->butPressedA && !dontInput && mCurrentSelected == 0)
				mCurrentState = 2, dontInput = true, mCurrentSelected = 0;

			if(mCurrentSelected < 0)
				mCurrentSelected = 0;
			mCurrentSelected = 0;
		}
		
		ulSetAlpha(UL_FX_ALPHA, alphaBlink, 1);
		ulDrawGradientRect(76, 108+mCurrentSelected*20, 256-76, 108+mCurrentSelected*20+12, RGB15(28, 28, 28), RGB15(28, 28, 28), RGB15(5, 15, 25), RGB15(5, 15, 25) );
		ulSetAlpha(UL_FX_DEFAULT, 0, 0);

		//LowLevel::drawRect(76, 108+mCurrentSelected*20, 256-76, 108+mCurrentSelected*20+12, RGB15(0, 15, 25) );

		//Menus
		LowLevel::drawString(256/2-LowLevel::getStringWidth("Play")/2, 110, "Play", 32767);
		//LowLevel::drawString(256/2-LowLevel::getStringWidth("Instructions")/2, 130, "Instructions", 32767);
		//LowLevel::drawString(256/2-LowLevel::getStringWidth("Options")/2, 150, "Options", 32767);
	}
	//-- Ecran selection de niveaux
	else if(mCurrentState == 2)
	{
		//Curseur
		if(!dontInput) 
		{
			if(input->butPressedUp)
				mCurrentSelected--, LowLevel::SoundManager::getInstance()->PlaySoundSimple(SND_MENU);
			if(input->butPressedDown)
				mCurrentSelected++, LowLevel::SoundManager::getInstance()->PlaySoundSimple(SND_MENU);
			
			//Selection menus
			if(input->butPressedA && !dontInput && mCurrentSelected == 1)
				mCurrentState = 1, dontInput = true, mCurrentSelected = 0;

			//Changement niveau courant
			if(mCurrentSelected == 0)
			{
				if(input->butPressedLeft)
					mSelectedLevel--, LowLevel::SoundManager::getInstance()->PlaySoundSimple(SND_MENU);
				if(input->butPressedRight)
					mSelectedLevel++, LowLevel::SoundManager::getInstance()->PlaySoundSimple(SND_MENU);
				
				if(mSelectedLevel < 0)
					mSelectedLevel = 0;
				if(mSelectedLevel >= grantedLevel)
					mSelectedLevel = grantedLevel;
				
				//Chargemmetn du niveau
				if(input->butPressedA && !dontInput)
				{
					activateFadeBlack = true;
					LowLevel::SoundManager::getInstance()->PlaySoundSimple(SND_MENU);
				}
			}

			if(mCurrentSelected < 0)
				mCurrentSelected = 1;
			mCurrentSelected = mCurrentSelected%2;
		}

		ulSetAlpha(UL_FX_ALPHA, alphaBlink, 1);
		ulDrawGradientRect(46, 108+mCurrentSelected*60, 256-46, 108+mCurrentSelected*60+12, RGB15(28, 28, 28), RGB15(28, 28, 28), RGB15(5, 15, 25), RGB15(5, 15, 25) );
		ulSetAlpha(UL_FX_DEFAULT, 0, 0);

		//Selection de niveaux
		LowLevel::drawString(256/2-LowLevel::getStringWidth("<- Select Level ->")/2, 110, "<- Select Level ->", 32767);
		
		//Affichage du niveau courant
		char levelStr[32];
		sprintf(levelStr, "Level %d/4", mSelectedLevel);

		LowLevel::drawString(256/2-LowLevel::getStringWidth(levelStr)/2, 130, levelStr, RGB15(0, 0, 31) );
		LowLevel::drawString(256/2-LowLevel::getStringWidth(levelsStr[mSelectedLevel])/2, 140, levelsStr[mSelectedLevel], RGB15(0, 0, 0) );
		
		//Calcul du temps du niveau
		int minutes, secondes, centiemes;
		centiemes = levelsScores[mSelectedLevel]%60;
		secondes = ((levelsScores[mSelectedLevel]-centiemes)/60)%60;
		minutes = ((levelsScores[mSelectedLevel]-centiemes)/60)/60;

		char time[32] = "";
		if(levelsScores[mSelectedLevel] != 0)
			sprintf(time, "Best time : %02d:%02d:%02d - Gems : %d/%d", minutes, secondes, (int)((float)centiemes*100.0f/60.0f), levelsGems[mSelectedLevel], levelsGemsMax[mSelectedLevel]);
		else
			sprintf(time, "Best time : --:--:-- - Gems : --/%d", levelsGemsMax[mSelectedLevel]);

		LowLevel::drawString(256/2-LowLevel::getStringWidth(time)/2, 150, time, 32767);

		LowLevel::drawString(256/2-LowLevel::getStringWidth("Back")/2, 170, "Back", 32767);
	}


	//Barres
	LowLevel::drawRect(0, 0, 256, 10, 0);
	LowLevel::drawRect(0, 192-10, 256, 192, 0);

	//Credits
	LowLevel::drawString(256/2-LowLevel::getStringWidth("by Morukutsu")/2, 183, "by Morukutsu", 32767);
	
	if(activateFadeBlack)
	{
		LowLevel::drawRect(0, 0, 256, 192, 0, fadeAlpha);
		fadeAlpha += 1;
		if(fadeAlpha == 32)
		{
			//Création du chemin
			char levelFilename[256];
			sprintf(levelFilename, "efs:/data/maps/level%d.tmx", mSelectedLevel);

			//Démarrage de la level state avec le chemin
			game->ChangeState(LevelState::Instance() );
			LevelState::Instance()->worldMan->LoadMap(levelFilename);
			LevelState::Instance()->worldMan->mLevelTime = levelTimes[mSelectedLevel];
			LevelState::Instance()->worldMan->mLevelBest = levelsScores[mSelectedLevel];
			LevelState::Instance()->worldMan->mLevelID = mSelectedLevel;
		}
	}

	dontInput = false;
}
