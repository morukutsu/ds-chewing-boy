/* --------------------------- */
/* Natura no Story             */
/* --------------------------- */

#ifndef H_DRAWABLE
#define H_DRAWABLE

#include "Image.h"

namespace LowLevel
{
    class Image;
    class Drawable
    {
    public:
        virtual void Draw() = 0;

        void Assign(Image* img)
        {
            image = img;
            scaleX = image->width;
            scaleY = image->height;
            isTexture = true;
        }
        void Scale(int w, int h)
        {
            scaleX = w;
            scaleY = h;
        };
        void Rotate(int Angle)
        {
            angle = Angle;
        };
        void MoveTo(int x, int y)
        {
            posX = x;
            posY = y;
        };
        void Translate(int x, int y)
        {
            posX += x;
            posY += y;
        };
        void SetRotCenter(int x, int y)
        {
            centerX = x;
            centerY = y;
        };
        void Mirror(bool v, bool h)
        {
            mirrorX = v;
            mirrorY = h;
            if (v == true)
                mirrorXVal = 16384;
            if (v == false)
                mirrorXVal = 0;
            if (h == true)
                mirrorYVal = 16384;
            if (h == false)
                mirrorYVal = 0;
        };
        void Alpha(int val, int group)
        {
            alpha = val;
            polygroup = group;
        };

    public:
        Image * image;                  //Pointeur vers l'image
        int centerX, centerY;           //Centre de rotation et de scaling
        int posX, posY;                 //Position X dans le repère
        int scaleX, scaleY;             //Facteur de redimensionnement de l'image
        int angle;                      //Angle de rotation
        bool mirrorX, mirrorY;          //Mirroir du sprite
        int mirrorXVal, mirrorYVal;     //Valleur du mirror
        int alpha, polygroup;
        int isTexture;
    };
}

#endif
