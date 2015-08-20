/* --------------------------- */
/* Natura no Story             */
/* --------------------------- */

//Includes
#include "MenuState.h"
#include "PauseState.h"
#include "LevelState.h"
#include "System/RessourceManager.h"
#include "StateManager.h"

//GFX
PauseState PauseState::m_PauseState;

void PauseState::Init()
{
	mCurrentSelected = 0;
}

void PauseState::Cleanup()
{

}

void PauseState::Pause()
{

}

void PauseState::Resume()
{

}

void PauseState::HandleEvents(StateManager* game)
{
    
}

void PauseState::Update(StateManager* game)
{

}

void PauseState::Draw(StateManager* game)
{
	//On récupère l'actions sur le menu
    LowLevel::Input *input = LowLevel::InputManager::getInstance()->GetInput();
	
	if(input->butPressedUp || input->butPressedX)
	{
		mCurrentSelected = 0;
	}
	if(input->butPressedDown || input->butPressedB)
	{
		mCurrentSelected = 1;
	}

	if(mCurrentSelected == 0 && (input->butPressedA || input->butPressedLeft) )
		game->PopState();
	else if(mCurrentSelected == 1 && (input->butPressedA || input->butPressedLeft) )
	{
		game->PopState();
		game->ChangeState(MenuState::Instance() );
	}

	LowLevel::drawString(256/2-LowLevel::getStringWidth("Pause")/2, 192/2-8, "Pause", 32767);
	
	LowLevel::drawRect(20, mCurrentSelected*15+140-2, 256-20, mCurrentSelected*15+150, RGB15(12, 12, 12) );
	LowLevel::drawString(256/2-LowLevel::getStringWidth("Resume")/2, 140, "Resume", 32767);
	LowLevel::drawString(256/2-LowLevel::getStringWidth("Back to main menu")/2, 155, "Back to main menu", 32767);
	
}
