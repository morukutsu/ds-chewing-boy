/* --------------------------- */
/* Natura no Story             */
/* --------------------------- */

//Includes
#ifndef _LEVELSTATE_H
#define _LEVELSTATE_H

#include "gamestate.h"
#include "Platform.h"

class WorldManagerOffline;
class LevelState : public CGameState
{
public:
    void Init();
    void Cleanup();

    void Pause();
    void Resume();

    void HandleEvents(StateManager * game);
    void Update(StateManager * game);
    void Draw(StateManager * game);

    static LevelState* Instance()
    {
        return &m_LevelState;
    }

	WorldManagerOffline* worldMan;

protected:
    LevelState() { }

private:
    static LevelState m_LevelState;
	
	bool lang;
};

#endif
