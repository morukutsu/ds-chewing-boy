/* --------------------------- */
/* Natura no Story             */
/* --------------------------- */

#ifndef H_SPRITE
#define H_SPRITE

#include "Drawable.h"
#include <ulib/ulib.h>

#define ANIM_LOOP        0
#define ANIM_ONCE        1

namespace LowLevel
{
    struct sSpriteAnim
    {
        //L'unité des positon est un multiple de la taille des frames
        u8 posy;           //Ligne de l'animation
        u8 startx;         //Colonne
        u8 lengh;          //Taille de l'animation
        u8 fps;            //Vitesse de l'animation en frames
    };

    class Sprite : public Drawable
    {
    public:
        Sprite();
        ~Sprite();

        void Draw();
        void Play();
        void Clip(int x, int y, int w, int h);
        void Tint(int color);

        int  AddAnimation(u8 py, u8 stx, u8 len, u8 fps);
        void PlayAnim(int mode, int anim);
        void SetAnimationSpeed(u8 animation, int speed);
        void SetFrameWH(int w, int h);
        void Pause();
        void Restart();
        void Stop();

    private:
        sSpriteAnim animations[32];   //Tableau d'animations du sprite
        int frameW, frameH;
        int animNbr, currentAnim, defaultAnim;   //Nombre d'animations dans le sprite
        u8 frameCounter, currentFrame;          //Compteur de frames et frame courante
        bool isStarted, isPaused, isLoop;       //Booléeins de depart et de pause pour les animations
		int col;

		//int old

    };

}

#endif
