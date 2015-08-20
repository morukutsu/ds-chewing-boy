/* --------------------------- */
/* Natura no Story             */
/* --------------------------- */

//Includes
#include "LevelState.h"
#include "PauseState.h"
#include "System/RessourceManager.h"
#include "StateManager.h"
#include "WorldManager.h"
#include "MenuState.h"

//GFX
LevelState LevelState::m_LevelState;

void LevelState::Init()
{
	setBrightness(2, 0);
    //Création du worldmanager
	worldMan = new WorldManagerOffline();
	worldMan->Init();
}

void LevelState::Cleanup()
{
	delete worldMan;
}

void LevelState::Pause()
{

}

void LevelState::Resume()
{

}

void LevelState::HandleEvents(StateManager* game)
{
    //On récupère l'actions sur le menu
    LowLevel::Input *input = LowLevel::InputManager::getInstance()->GetInput();

	if(input->butPressedStart)
		game->PushState(PauseState::Instance() );
	
}

void LevelState::Update(StateManager* game)
{
    worldMan->Update();

	//Fin de niveau
	if(worldMan->mFinishTimer >= 180)
	{	
		if(worldMan->mLevelFinished)
		{
			LowLevel::SoundManager::getInstance()->StopMusic();

			//Enregistrement du nouveau score
			int levelsScores[32], levelGems[32];
			FILE * f = fopen("fat:/cb_scores.sc", "rb");
			if(f)
			{
				fread(levelsScores, sizeof(int), 32, f);
				fread(levelGems, sizeof(int), 32, f);
				fclose(f);
			}

			//Enregistrement
			if(worldMan->mFrameCounter <= levelsScores[worldMan->mLevelID] || levelsScores[worldMan->mLevelID] == 0)
			{
				levelsScores[worldMan->mLevelID] = worldMan->mFrameCounter;
				levelGems[worldMan->mLevelID] = worldMan->mLevelGemmesCount;
				f = fopen("fat:/cb_scores.sc", "wb+");
				if(f)
				{
					fwrite(levelsScores, sizeof(int), 32, f);
					fwrite(levelGems, sizeof(int), 32, f);
					fclose(f);
				}
			}
		}
		game->ChangeState(MenuState::Instance() );
	}
}

void LevelState::Draw(StateManager* game)
{
	 worldMan->Draw();
}