/* --------------------------- */
/* ITM2                        */
/* --------------------------- */

#ifndef H_SUB2D
#define H_SUB2D

#include <nds.h>

namespace LowLevel
{
	//Sprites
	class Sprite2D
    {
    public:
        Sprite2D();
        ~Sprite2D();
		
		void Assign(sImage* img);
        void Draw();
		void MoveTo(int x, int y);
        void Tint(int color);
		SpriteSize GetSpriteSize(int w, int h);

    private:
		int x, y;
		u16* gfx;
		SpriteColorFormat format;
		SpriteSize size;
		sImage* img;
		int spid;

		static int spn;
    };
};

#endif