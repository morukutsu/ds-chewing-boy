/* --------------------------- */
/* Natura no Story             */
/* --------------------------- */

#ifndef H_MODEL
#define H_MODEL

#include "Drawable.h"
#include "FixedPoint.h"
#include "MD2.h"
#include <ulib/ulib.h>
#include <vector>

#define ANIM_LOOP        0
#define ANIM_ONCE        1

#define ONCE_STAYLAST    0
#define ONCE_STAYFIRST   1

namespace LowLevel
{
    struct sModelAnim
    {
        //L'unité des positon est un multiple de la taille des frames
        u8  startFrame;       //Frame de départ
        u8  endFrame;         //Frame d'arrivée
        u8  fps;              //Vitesse de l'animation en frames
        int pas;              //Pas de l'animation (pour l'interpolation)
    };

    class Model : public Drawable
    {
    public:
        Model();
        ~Model();

        void Draw();

        void Play();
        void LoadModel(char* filename, int widthheight, int scale);
        void AssignModel(MD2Entity* Model);

        int  AddAnim(u8 sta, u8 end, u8 fps);
        void PlayAnim(int mode, int anim, bool isTrans = true);
        void Interpolate(bool trigger);
        void SetAnimationSpeed(u8 animation, int speed);
        void SetOnceMonde(int mode);
        void Pause();
        void Restart();
        void Stop();

        void Tint(int color);
        void Outline(int color, bool trigger);
		void SetMode(bool mode) { mapMode = mode; }

        MD2Entity* model;

    private:
        int color;
        std::vector <sModelAnim> animations;   //Tableau d'animations du sprite
        u8 animNbr, defaultAnim;
        int currentAnim;
        u8 frameCounter, currentFrame;
        u8 interFrameCounter;
        bool isStarted, isPaused, isLoop, isStatic, isInterpolation, isOutline;
        int outlineColor;
        int onceMode;
		int transition, transTimer;
		bool isTransition;
		bool mapMode;

        //Gestion globale des modèles
        static bool isInited;
        //static v16 anormtable16[162][3];
    };

}

#endif
