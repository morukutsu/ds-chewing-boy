/*****************************/
/* Inside The Machine - MKS  */
/*****************************/

//Pompé sur le GCN, MERCI
#ifndef _GAMESTATE_H
#define _GAMESTATE_H

#include "StateManager.h"

class StateManager;
class CGameState
{
public:

    virtual void Init() = 0;
    virtual void Cleanup() = 0;

    virtual void Pause() = 0;
    virtual void Resume() = 0;

    virtual void HandleEvents(StateManager* game) = 0;
    virtual void Update(StateManager* game) = 0;
    virtual void Draw(StateManager* game) = 0;

    /*void ChangeState(StateManager* game, CGameState* state)
    {
        game->ChangeState(state);
    }*/

protected:
    CGameState() { }
};

#endif
