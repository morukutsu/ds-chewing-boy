/* --------------------------- */
/* Natura no Story             */
/* --------------------------- */

//Includes
#include "MenuState.h"
#include "SplashState.h"
#include "System/RessourceManager.h"
#include "StateManager.h"

//GFX
SplashState SplashState::m_SplashState;

void SplashState::Init()
{
	//Chargement des sprites
	setBrightness(2, -16);
	mSplash.Assign(System::CRessourceManager::getInstance()->LoadImage("efs:/data/backgrounds/splash.png", UL_IN_VRAM, UL_PF_PAL8) );
	mSplash.MoveTo(0, 0);
	mTime = 0;
	mAlpha = 31;
}

void SplashState::Cleanup()
{
	System::CRessourceManager::getInstance()->DeleteImage(mSplash.image);
}

void SplashState::Pause()
{

}

void SplashState::Resume()
{

}

void SplashState::HandleEvents(StateManager* game)
{
    
}

void SplashState::Update(StateManager* game)
{

}

void SplashState::Draw(StateManager* game)
{
	//On récupère l'actions sur le menu
    LowLevel::Input *input = LowLevel::InputManager::getInstance()->GetInput();

	//Fond
	mSplash.Draw();	
	if(mAlpha != 0)
		LowLevel::drawRect(0, 0, 256, 192, 32767, mAlpha);
	
	if(mTime < 31)
		mAlpha -= 1;
	
	if(mTime > 180)
		mAlpha += 1;

	if(mAlpha <= 0)
		mAlpha = 0;
	if(mAlpha >= 31)
		mAlpha = 31;
	
	if(mTime > 180+31)
		game->ChangeState(MenuState::Instance() );
	mTime++;
}
