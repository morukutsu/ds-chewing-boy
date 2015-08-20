/* --------------------------- */
/* Natura no Story             */
/* --------------------------- */

#include "3DWrapper.h"

namespace LowLevel
{
    void drawTri(int x0, int y0, int x1, int y1, int x2, int y2, int color)
    {
        //We're drawing a simple colored triangle (untextured)
        ulDisableTexture();

        //Begins drawing quadliterals
        ulVertexBegin(GL_TRIANGLES);

        //You can define the color of each vertex before issuing the vertex command, or once for all, like here. If the vertices have different colors, a gradient will appear between each corner.
        ulVertexColor(color);

        //Draw our triangle (3 vertices)
        ulVertexXY(x0, y0);
        ulVertexXY(x1, y1);
        ulVertexXY(x2, y2);

        //End our drawing.
        ulVertexEnd();

        //Don't forget to auto-increment depth
        ulVertexHandleDepth();
        ulVertexColor(32767);
        return;
    }

    void drawQuad(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, int color)
    {
        //We're drawing a simple colored triangle (untextured)
        ulDisableTexture();

        //Begins drawing quadliterals
        ulVertexBegin(GL_QUADS);

        //You can define the color of each vertex before issuing the vertex command, or once for all, like here. If the vertices have different colors, a gradient will appear between each corner.
        ulVertexColor(color);

        //Draw our triangle (3 vertices)
        ulVertexXY(x0, y0);
        ulVertexXY(x1, y1);
        ulVertexXY(x2, y2);
        ulVertexXY(x3, y3);

        //End our drawing.
        ulVertexEnd();

        //Don't forget to auto-increment depth
        ulVertexHandleDepth();
        ulVertexColor(32767);
        return;
    }

    void drawRect(int x0, int y0, int x1, int y1, int color, int alpha, int polygroup)
    {
		ulSetAlpha(UL_FX_ALPHA, alpha, polygroup);
        //We're drawing a simple colored triangle (untextured)
        ulDisableTexture();

        //Begins drawing quadliterals
        ulVertexBegin(GL_QUADS);

        //You can define the color of each vertex before issuing the vertex command, or once for all, like here. If the vertices have different colors, a gradient will appear between each corner.
        ulVertexColor(color);

        //Draw our triangle (3 vertices)
        ulVertexXY(x0, y0);
        ulVertexXY(x1, y0);
        ulVertexXY(x1, y1);
        ulVertexXY(x0, y1);

        //End our drawing.
        ulVertexEnd();

        //Don't forget to auto-increment depth
        ulVertexHandleDepth();
        ulVertexColor(32767);
		ulSetAlpha(UL_FX_DEFAULT, 0, 0);
        return;
    }
}
