/* --------------------------- */
/* Natura no Story             */
/* --------------------------- */

//Includes
#ifndef _SPLASHSTATE_H
#define _SPLASHSTATE_H

#include "gamestate.h"
#include "Platform.h"

class SplashState : public CGameState
{
public:
    void Init();
    void Cleanup();

    void Pause();
    void Resume();

    void HandleEvents(StateManager * game);
    void Update(StateManager * game);
    void Draw(StateManager * game);

    static SplashState* Instance()
    {
        return &m_SplashState;
    }

protected:
    SplashState() { }

private:
    static SplashState m_SplashState;
	
	LowLevel::Sprite mSplash;
	int mTime, mAlpha;
};

#endif
