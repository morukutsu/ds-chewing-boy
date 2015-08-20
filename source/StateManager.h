/*****************************/
/* Inside The Machine - MKS  */
/*****************************/

#ifndef _STATEMANAGER_H
#define _STATEMANAGER_H

//Includes generaux
#include <vector>
#include "gamestate.h"

class CGameState;
class StateManager
{
public:
    void Init();
    void cleanUp();
    void ChangeState(CGameState* state);
    void PushState(CGameState* state);
    void PopState();

    void HandleEvents();
    void Update();
    void Draw();

    bool Running();
    void Quit();

private:
    //La pile d'états
    std::vector<CGameState*> states;

    bool m_running;
};

#endif
