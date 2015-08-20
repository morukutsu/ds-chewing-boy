/* --------------------------- */
/* Natura no Story             */
/* --------------------------- */

#ifndef H_3DWRAPPER
#define H_3DWRAPPER

//Includes
#include <ulib/ulib.h>

//Wrapper des fonctions 3D de base
namespace LowLevel
{
    void drawTri(int x1, int y1, int x2, int y2, int x3, int y3, int color);
    void drawQuad(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, int color);
    void drawRect(int x0, int y0, int x1, int y1, int color, int alpha = 31, int pg = 30);
}

#endif
