/* --------------------------- */
/* Obsolete                    */
/* --------------------------- */

#include "Backbuffer.h"

u32 backbuffer[256][192] = {};
u32 lookupTable[256] = {0};
int bgID;

void InitBackBuffer()
{
	vramSetBankE(VRAM_E_MAIN_BG);
	bgID = bgInit(3, BgType_Bmp8, BgSize_B8_256x256, 0, 0);
	bgSetPriority(bgID, 3);
	bgSetPriority(0, 2);
	bgShow(3);
}

void ClearBackBuffer()
{
	for(int y = 0; y < 192; y++)
	{
		for(int x = 0; x < 256; x++)
		{
			backbuffer[x][y] = 0x7FFF;
		}
	}
	dmaCopy(backbuffer, bgGetGfxPtr(bgID), 256*256);
}

void InitBackBufferEffect(int effect)
{
	if(effect == EFFECT_FIRE)
	{
		for(int I = 0; I < 32; I++)
			BG_PALETTE[I] = RGB15(0, 0, 0); // Noir
		for(int I = 32; I < 64; I++)
			BG_PALETTE[I] = RGB15(I - 32, 0, 0); // 1/8 dégradé -> rouge
		for(int I = 64; I < 128; I++)
			BG_PALETTE[I] = RGB15(31, (I - 64) >> 1, 0);  // 1/4 dégradé -> jaune
		for(int I = 128; I < 256; I++)
			BG_PALETTE[I] = RGB15(31, 31, (I - 128) >> 2); // 1/2 dégradé -> blanc

	}
}

void UpdateBackBuffer(int effect)
{

}