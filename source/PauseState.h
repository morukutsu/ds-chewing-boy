/* --------------------------- */
/* Natura no Story             */
/* --------------------------- */

//Includes
#ifndef _PAUSESTATE_H
#define _PAUSESTATE_H

#include "gamestate.h"
#include "Platform.h"

class PauseState : public CGameState
{
public:
    void Init();
    void Cleanup();

    void Pause();
    void Resume();

    void HandleEvents(StateManager * game);
    void Update(StateManager * game);
    void Draw(StateManager * game);

    static PauseState* Instance()
    {
        return &m_PauseState;
    }

protected:
    PauseState() { }

private:
    static PauseState m_PauseState;
	int mCurrentSelected;
	
};

#endif
