/* --------------------------- */
/* Natura no Story             */
/* --------------------------- */

#include "Platform.h"
#include "LevelState.h"
#include "MenuState.h"
#include "SplashState.h"
#include "System/RessourceManager.h"
#include "System/autoint.h"

int main()
{
    //Initialisation de la plateforme
	defaultExceptionHandler();
    LowLevel::Platform::getInstance()->Init();

    //Déclaration du state manager
    StateManager* stman = new StateManager;
    stman->ChangeState(SplashState::Instance() );
	//stman->ChangeState(MenuState::Instance() );

	//Initialisation du son
	LowLevel::SoundManager::getInstance()->Init();
    //Déclaration du FX manager	
	//Préchargements
	//System::CRessourceManager::getInstance()->LoadImage("efs:/data/sprites/spr_hero.png", UL_IN_VRAM, UL_PF_PAL4);
	LowLevel::ParticleGenerator::getInstance()->Init();

    int totalTime = 1;

    //Boucle Principale
    while (1)
    {
        //Initialize the timers to measure the framerate
        TIMER1_CR = 0;
        TIMER1_DATA = 0;
        TIMER1_CR = TIMER_DIV_64 | TIMER_ENABLE;
		bgUpdate();

        //Récupération de l'input
        LowLevel::InputManager::getInstance()->Handle();

        //Boucle de mise à jour
		//mmStreamUpdate();
        stman->HandleEvents();
        stman->Update();

        //Boucle d'affichage
        LowLevel::Platform::getInstance()->BeginDisplay();
        //FX2::getInstance()->DrawBG();
        stman->Draw();
        //FX2::getInstance()->DrawParticles();
        //FX2::getInstance()->DrawTransition();
		//FX2::getInstance()->DrawHits();
        //FX2::getInstance()->DrawShader();
		//FX2::getInstance()->Handle();


        //O - Debug
        ulSetTextColor(RGB15(31, 0, 0));
        //ulPrintf_xy(1, 1, "Free Vram memory : %d ko", ulGetTexVramAvailMemory() / 1024);
        int cpuTime = (TIMER1_DATA * 1000) / totalTime;
        //ulPrintf_xy(5, 10, "CPU: %02i.%i%%", cpuTime / 10, cpuTime % 10);
        //ENd - Debug

        LowLevel::Platform::getInstance()->EndDisplay();
        LowLevel::Platform::getInstance()->Sync();
        totalTime = TIMER1_DATA;
    }

    //Déchargement
    LowLevel::Platform::getInstance()->Unload();

    return 0;
}
