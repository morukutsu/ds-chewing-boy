/* --------------------------- */
/* Natura no Story             */
/* --------------------------- */

#ifndef H_IMAGE
#define H_IMAGE

#include <ulib/ulib.h>

extern char pngbuffer[16*1024];

namespace LowLevel
{
    //La classe image repr�sente juste une texture en m�moire, elle ne devrait pas �tre affich�e
    //sans un sprite pour la contenir.
    class Image
    {
    public:
        Image();
		~Image();
        void SetImage(UL_IMAGE *img);
        void SetManagerLoad();
        void FreeImage();

        void PNGLoadFromFS(char* filename, int location, int pixelFormat);

    public:
        UL_IMAGE* img;
        int height, width;
        int colorKey;
        bool isLoadedWithRessourceManager;
    };
}

#endif
