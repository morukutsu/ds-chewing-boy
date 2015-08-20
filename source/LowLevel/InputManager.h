/* --------------------------- */
/* Natura no Story             */
/* --------------------------- */

#ifndef H_INPUTMANAGER
#define H_INPUTMANAGER

//Includes generaux
#include "../Platform.h"
#include "../system/singleton.h"
#include <ulib/ulib.h>

#define BUT_NONE          -1
#define BUT_PUSHED         0
#define BUT_HELD           1
#define BUT_RELEASED       2

namespace LowLevel
{
    using namespace std;
    struct Input
    {
        bool butHeldStart, butHeldSelect, butHeldTouch, butHeldA, butHeldB, butHeldX, butHeldY, butHeldL, butHeldR, butHeldUp, butHeldDown, butHeldLeft, butHeldRight;
        bool butPressedStart, butPressedSelect, butPressedTouch, butPressedA, butPressedB, butPressedX, butPressedY, butPressedL, butPressedR, butPressedUp, butPressedDown, butPressedLeft, butPressedRight;
        bool butReleasedStart, butReleasedSelect, butReleasedTouch, butReleasedA, butReleasedB, butReleasedX, butReleasedY, butReleasedL, butReleasedR, butReleasedUp, butReleasedDown, butReleasedLeft, butReleasedRight;

        int touchX, touchY;
    };

    class InputManager : public Singleton<InputManager>
    {
        friend class Singleton<InputManager>;
        //Fonctions
    public:
        InputManager();                  //Constructeur
        ~InputManager();                 //Destructeur

        void Handle();
		void Blank(Input* input);
        Input* GetInput();

        //Variables
    private:
        Input input;                     //Variable Input

    };
};
#endif
