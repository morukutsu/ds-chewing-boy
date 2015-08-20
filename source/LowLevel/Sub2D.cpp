/* --------------------------- */
/* ITM2                        */
/* --------------------------- */

#include "Sub2D.h"
#include "Platform.h"

namespace LowLevel
{
	int Sprite2D::spn = 0;
	Sprite2D::Sprite2D()
	{
		spid = spn;
		spn++;
	}

	Sprite2D::~Sprite2D()
	{

	}
		
	void Sprite2D::Assign(sImage* img)
	{
		size = GetSpriteSize(img->width, img->height);
		gfx = oamAllocateGfx(&oamSub, size, SpriteColorFormat_256Color);
		format = SpriteColorFormat_256Color;

		// Sprite initialisation
		/*for(int i = 0; i < 256; i++)
			SPRITE_PALETTE_SUB[i] = img->palette[i];*/

		for(int i = 0; i < (img->width*img->height)>>1; i++)
			gfx[i] = img->image.data16[i];
		//dmaCopy(img->image.data16, gfx, 32*16);
		//swiCopy(img->image.data16, gfx, ((sz & 0xFFF) << 4) | COPY_MODE_FILL);
	}

    void Sprite2D::Draw()
	{
		oamSet(&oamSub, 
			spid, 
			x, y, 
			0, 
			0,
			size,
			format, 
			gfx, 
			-1, 
			false, 
			false,
			false,
			false, 
			false);
	}
 
	void Sprite2D::MoveTo(int px, int py)
	{
		x = px;
		y = py;
	}
    
    void Sprite2D::Tint(int color)
	{

	}

	SpriteSize Sprite2D::GetSpriteSize(int w, int h)
	{
		if(w == 8)
		{
			if(h == 8)
				return SpriteSize_8x8;
			if(h == 16)
				return SpriteSize_8x16;
			if(h == 32)
				return SpriteSize_8x32;
		}
		if(w == 16)
		{
			if(h == 16)
				return SpriteSize_16x16;
			if(h == 8)
				return SpriteSize_16x8;
			if(h == 32)
				return SpriteSize_16x32;
		}
		if(w == 32)
		{
			if(h == 32)
				return SpriteSize_32x32;
			if(h == 8)
				return SpriteSize_32x8;
			if(h == 16)
				return SpriteSize_32x16;
			if(h == 64)
				return SpriteSize_32x64;
		}
		if(w == 64)
		{
			if(h == 64)
				return SpriteSize_64x64;
			if(h == 32)
				return SpriteSize_64x32;
		}
	}
};