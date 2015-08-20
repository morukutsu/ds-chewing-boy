/* --------------------------- */
/* Natura no Story             */
/* --------------------------- */

//Includes
#ifndef _MENUSTATE_H
#define _MENUSTATE_H

#include "gamestate.h"
#include "Platform.h"

class MenuState : public CGameState
{
public:
    void Init();
    void Cleanup();

    void Pause();
    void Resume();

    void HandleEvents(StateManager * game);
    void Update(StateManager * game);
    void Draw(StateManager * game);

    static MenuState* Instance()
    {
        return &m_MenuState;
    }

protected:
    MenuState() { }

private:
    static MenuState m_MenuState;
	
	LowLevel::Sprite mFond;
	bool lang, dontInput;

	int mCurrentState, mCurrentSelected, mSelectedLevel, grantedLevel;
	int alphaBlink;
	bool alphaSens;

	char* levelsStr[32];
	int levelsScores[32];
	int levelsGems[32];
	int levelsGemsMax[32];
	int levelTimes[32];
	int fadeAlpha;
	bool activateFadeBlack;
};

#endif
