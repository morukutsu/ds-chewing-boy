/* --------------------------- */
/* Natura no Story             */
/* --------------------------- */

//Includes
#include "InputManager.h"

namespace LowLevel
{
    InputManager::InputManager()
    {


    }

    InputManager::~InputManager()
    {

    }

    void InputManager::Handle()
    {
        //On met a jour l'état de l'input
        ulReadKeys(0);
		//PA_UpdatePad();
		//PA_UpdateStylus();
        memset(&input, 0, sizeof(Input));

        //Touch
        input.touchX = ul_keys.touch.x;
        input.touchY = ul_keys.touch.y;

        //Start
        if (ul_keys.pressed.start)
            input.butPressedStart = true;
        if (ul_keys.released.start)
            input.butReleasedStart = true;
        if (ul_keys.held.start)
            input.butHeldStart = true;

        //Select
        if (ul_keys.pressed.select)
            input.butPressedSelect = true;
        if (ul_keys.released.select)
            input.butReleasedSelect = true;
        if (ul_keys.held.select)
            input.butHeldSelect = true;

        //Touch
        if (ul_keys.touch.click)
            input.butPressedTouch = true;
        if (ul_keys.touch.released)
            input.butReleasedTouch = true;
        if (ul_keys.touch.held)
            input.butHeldTouch = true;

        //A
        if (ul_keys.pressed.A)
            input.butPressedA = true;
        if (ul_keys.released.A)
            input.butReleasedA = true;
        if (ul_keys.held.A)
            input.butHeldA = true;

        //B
        if (ul_keys.pressed.B)
            input.butPressedB = true;
        if (ul_keys.released.B)
            input.butReleasedB = true;
        if (ul_keys.held.B)
            input.butHeldB = true;

        //X
        if (ul_keys.pressed.X)
            input.butPressedX = true;
        if (ul_keys.released.X)
            input.butReleasedX = true;
        if (ul_keys.held.X)
            input.butHeldX = true;

        //Y
        if (ul_keys.pressed.Y)
            input.butPressedY = true;
        if (ul_keys.released.Y)
            input.butReleasedY = true;
        if (ul_keys.held.Y)
            input.butHeldY = true;

		//L
        if (ul_keys.pressed.L)
            input.butPressedL = true;
        if (ul_keys.released.L)
            input.butReleasedL = true;
        if (ul_keys.held.L)
            input.butHeldL = true;

		//R
        if (ul_keys.pressed.R)
            input.butPressedR = true;
        if (ul_keys.released.R)
            input.butReleasedR = true;
        if (ul_keys.held.R)
            input.butHeldY = true;

        //Up
        if (ul_keys.pressed.up)
            input.butPressedUp = true;
        if (ul_keys.released.up)
            input.butReleasedUp = true;
        if (ul_keys.held.up)
            input.butHeldUp = true;

        //Down
        if (ul_keys.pressed.down)
            input.butPressedDown = true;
        if (ul_keys.released.down)
            input.butReleasedDown = true;
        if (ul_keys.held.down)
            input.butHeldDown = true;

        //Left
        if (ul_keys.pressed.left)
            input.butPressedLeft = true;
        if (ul_keys.released.left)
            input.butReleasedLeft = true;
        if (ul_keys.held.left)
            input.butHeldLeft = true;

        //Right
        if (ul_keys.pressed.right)
            input.butPressedRight = true;
        if (ul_keys.released.right)
            input.butReleasedRight = true;
        if (ul_keys.held.right)
            input.butHeldRight = true;
    }

    Input* InputManager::GetInput()
    {
        return &input;
    }

	void InputManager::Blank(Input* input2)
	{
		memset(input2, 0, sizeof(Input));
	}
};
