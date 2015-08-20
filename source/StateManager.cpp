/*****************************/
/* Inside The Machine - MKS  */
/*****************************/

//Includes
#include "StateManager.h"

void StateManager::Init()
{
    m_running = true;
}
bool StateManager::Running()
{
    return m_running;
}
void StateManager::Quit()
{
    m_running = false;
}
void StateManager::cleanUp()
{
    //Nettoie le pile des states
    while ( !states.empty() )
    {
        states.back()->Cleanup();
        states.pop_back();
    }
    //std::cout << "Debug : Pile de states nettoyée, fin imminente" << std::endl;
}
void StateManager::ChangeState(CGameState* state)
{
    //Nettoie le state
    if ( !states.empty() )
    {
        states.back()->Cleanup();
        states.pop_back();
    }

    //Ajoute et initialise le nouvel état
    states.push_back(state);
    states.back()->Init();
}

void StateManager::PushState(CGameState* state)
{
    //Met en pause l'état courant
    if ( !states.empty() )
    {
        states.back()->Pause();
    }

    //Ajoute et initialise le nouvel état
    states.push_back(state);
    states.back()->Init();
}

void StateManager::PopState()
{
    //Vide l'état courant
    if ( !states.empty() )
    {
        states.back()->Cleanup();
        states.pop_back();
    }

    //Restaurationn de l'état précédent
    if ( !states.empty() )
    {
        states.back()->Resume();
    }
}


void StateManager::HandleEvents()
{
    //let the state handle events
    states.back()->HandleEvents(this);
}

void StateManager::Update()
{
    // let the state update the game
    states.back()->Update(this);
}

void StateManager::Draw()
{
    // let the state draw the screen
    states.back()->Draw(this);
}
