/* --------------------------- */
/* Natura no Story             */
/* --------------------------- */

#include "Platform.h"
//#include "../soundbank.h"
#include "../gfx/all_gfx.c"
#include "../gfx/all_gfx.h"

unsigned int VCountS = 0;
unsigned int VCountE = 0;

void VCountHandler(void);

void VCountHandler(void)
{
	//VCountS = REG_VCOUNT;
	//mmStreamUpdate();
	//VCountE = REG_VCOUNT;
	//if( VCountE < VCountS )
	//	VCountS = ( 263 - VCountS ) + VCountE;
	//else
	//	VCountS = VCountE - VCountS;
	//VCountS = ( VCountS * 12 ) >> 5;
}

#include <malloc.h>    // for mallinfo()
#include <unistd.h>    // for sbrk()

extern u8 __end__[];        // end of static code and data
extern u8 __eheap_end[];    // farthest point to which the heap will grow

u8* getHeapStart() {
        return __end__;
}

u8* getHeapEnd() {
        return (u8*)sbrk(0);
}

u8* getHeapLimit() {
        return __eheap_end;
}

int getMemUsed() {    // returns the amount of used memory in bytes
        struct mallinfo mi = mallinfo();
        return mi.uordblks;
}

int getMemFree() {    // returns the amount of free memory in bytes
        struct mallinfo mi = mallinfo();
        return mi.fordblks + (getHeapLimit() - getHeapEnd());
}

void HBLFunc()
{
	s16 vcount = REG_VCOUNT;
	//REG_BG0HOFS  = 16384*10;
	//REG_BG0HOFS = -128+1*vcount;
	//Gestion des effets
	if(LowLevel::Platform::sineWaveActivated)
		REG_BG0HOFS = sinLerp(  ( ((vcount + (LowLevel::Platform::hbladd>>8))  * LowLevel::Platform::sineWaveMult) & LUT_MASK ) << 6) >> LowLevel::Platform::sineWaveIntensity; 
}

void VBLFunc()
{
	//Incrémentation de l'hbladd pour l'effet sinewave
	LowLevel::Platform::hbladd += LowLevel::Platform::sineWaveSpeed;
}

namespace LowLevel
{
	//u8 soundData[784*1024];
	int  Platform::hbladd = 0;
	int  Platform::sineWaveMult, Platform::sineWaveIntensity, Platform::sineWaveSpeed;
	bool Platform::sineWaveActivated = false;
	OamState* Platform::oam = &oamSub;
	int Platform::paletteOffset = 0;

	void Platform::ActiveEffectSineWave(bool t, int speed, int mult, int intensity) //256, 10 , 9
	{
		sineWaveActivated = t;
		sineWaveSpeed = speed;
		sineWaveMult = mult;
		sineWaveIntensity = intensity;
	}

    Platform::Platform()
    {

    }

    void Platform::Init()
    {
        //Main & GFX init
        //PA_Init();
        //PA_InitVBL();
		irqSet(IRQ_VBLANK, VBLFunc);
		irqEnable(IRQ_VBLANK);
		irqSet(IRQ_HBLANK, HBLFunc);
		irqEnable(IRQ_HBLANK);

		//consoleDemoInit();
		//PA_Init();    // Initializes PA_Lib
		//PA_InitVBL(); // Initializes a standard VBL

        ulInit(UL_INIT_LIBONLY);
        ulInitGfx();
        ulInitText();
		ulSetMainLcd(1);

        ulSetTexVramParameters(UL_BANK_A | UL_BANK_B , VRAM_A, 256 << 10);

		/** test 2D **/
		//BG
		/*videoSetModeSub(MODE_5_2D);
		int bg3 = bgInitSub(3, BgType_Bmp8, BgSize_B8_256x256, 0,0);
		vramSetBankC(VRAM_C_SUB_BG_0x06200000);
		dmaCopy(natura_bottomBitmap, bgGetGfxPtr(bg3), 256*256);
		dmaCopy(natura_bottomPal, BG_PALETTE_SUB, 256*2);*/

		//SPRITES
		/*vramSetBankD(VRAM_D_SUB_SPRITE);
		oamInit(&oamSub,  SpriteMapping_1D_32, false);
		//Chargemment des différents sprites
		gfxFleche = oamAllocateGfx(&oamSub, SpriteSize_64x64, SpriteColorFormat_256Color);
		for(int i = 0; i < EndTilesLen / 2; i++) { gfxFleche[i] = EndTiles[i]; }
		for(int i = 0; i < EndPalLen / 2; i++) { SPRITE_PALETTE_SUB[i] = EndPal[i]; }*/
		/** fin test 2d **/

        //vramSetMainBanks(VRAM_A_TEXTURE, VRAM_B_TEXTURE, VRAM_C_MAIN_BG, VRAM_D_MAIN_BG);
        //ulEnableEdgeAntialiasing();
		//glPolyFmt(POLY_ALPHA(31) | POLY_CULL_BACK );
		
        EFS_Init(EFS_AND_FAT, NULL);
		
		//Init MaxMod
		mmInitDefault("efs:/soundbank.bin");

		/*mm_ds_system sys;
		sys.mod_count		= 0;
		sys.samp_count		= 0;
		sys.mem_bank		= 0;
		sys.fifo_channel	= FIFO_MAXMOD;
		mmInit( &sys );

		mmSoundBankInFiles("efs:/soundbank.bin");*/

		//mmInitDefault("efs:/soundbank.bin");
		

		//irqSet( IRQ_VCOUNT, VCountHandler );
		//irqEnable( IRQ_VCOUNT );

		// Init AS_Lib for mp3
		//PA_VBLFunctionInit(AS_SoundVBL);
		
		//AS_Init(AS_MODE_MP3 | AS_MODE_SURROUND | AS_MODE_16CH);
		//AS_SetDefaultSettings(AS_PCM_8BIT, 11025, AS_SURROUND);
        //PA_InitASLibForMP3(AS_MODE_MP3 | AS_MODE_SURROUND | AS_MODE_16CH);
        //AS_SetMP3Loop(true);
		//AS_SetMP3Loop(true);
        //AS_SetDefaultSettings(AS_PCM_8BIT, 11025, AS_NO_DELAY);
		//PA_VBLFunctionInit(AS_SoundVBL);
		//AS_Init(AS_MODE_MP3 | AS_MODE_SURROUND | AS_MODE_16CH);  // initializes AS_Lib
		//AS_SetDefaultSettings(AS_PCM_8BIT, 11025, AS_SURROUND);  // or your preferred default sound settings

		//AS_MP3StreamPlay("efs:/Plain.mp3");

		//Chargemment de GFX
		//PA_EasyBgLoad(1, 3, Hero); //Background sur l'écran inférieur
		//PA_LoadSpritePal(1, 0, (void*)Pointer_Pal);
		//PA_CreateSprite(1, 0, (void*)Pointer_Sprite, OBJ_SIZE_16X16, 1, 0, -50, -50); // Pointeur écran tactile
		
		glEnable(GL_ANTIALIAS);
		glClearColor(0, 0, 0, 31); // BG must be opaque for AA to work
		glClearPolyID(63); // BG must have a unique polygon ID for AA to work
		glClearDepth(0x7FFF);

		//Initialisation du subscreen2d
		videoSetModeSub(MODE_5_2D | DISPLAY_BG3_ACTIVE | DISPLAY_SPR_ACTIVE | DISPLAY_SPR_2D_BMP_256);
		vramSetBankD(VRAM_D_SUB_SPRITE);
		oamInit(&oamSub, SpriteMapping_1D_128, false);
	}

    void Platform::Unload()
    {

    }

    void Platform::BeginDisplay()
    {
        ulStartDrawing2D();
        //glClearPolyID(8);
    }

    void Platform::EndDisplay()
    {
        ulEndDrawing();
    }

    void Platform::Sync()
    {
        //ulEndFrame();
		ulEndFrame();
		//PA_WaitForVBL();
		ulSyncFrame();
		oamUpdate(&oamSub);
		LowLevel::SoundManager::getInstance()->Handle();
		
        //PA_WaitForVBL();
    }

    //Fonctions
    bool uStylusInZone(u16 x1, u16 y1, u16 x2, u16 y2)
    {
        if (ul_keys.touch.x  < x1)
            return 0;
        if (ul_keys.touch.x  > x1 + (x2-x1))
            return 0;
        if (ul_keys.touch.y  < y1)
            return 0;
        if (ul_keys.touch.y  > y1 + (y2-y1))
            return 0;

        return 1;
    }

    bool RectCollision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2)
    {
        if (x1+w1<x2) return false;
        if (x2+w2<x1) return false;
        if (y1+h1<y2) return false;
        if (y2+h2<y1) return false;

        //On arrive là, il y a donc collision entre les deux rectangles
        return true;
    }

	bool AABBCollide(int px1, int py1, sHitBoxRect* a, int px2, int py2, sHitBoxRect* b)
	{
		if(px1 + a->x2 < px2 + b->x1 || 
					py1 + a->y2 < py2 + b->y1 || 
					px1 + a->x1 > px2 + b->x2 || 
					py1 + a->y1 > py2 + b->y2) 
		{
			return false;
		}
		return true;
	}

    bool RectPointCollision(int x1, int y1, int x2, int y2, int w2, int h2)
    {
        if (x1<x2) return false;
        if (x2+w2<x1) return false;
        if (y1<y2) return false;
        if (y2+h2<y1) return false;

        //On arrive là, il y a donc collision entre le rectangle et le point
        return true;
    }

	bool RectPointCollision(int x1, int y1, int x2, int y2, sHitBoxRect * a)
    {
        if (x1 < (x2 + a->x1)) return false;
        if ((x2+a->x1) + (a->x2-a->x1) < x1) return false;
        if (y1 < (y2 + a->y1)) return false;
        if ((y2+a->y1) + (a->y2-a->y1) < y1) return false;

        //On arrive là, il y a donc collision entre le rectangle et le point
        return true;
    }

	bool RectCollision(int px1, int py1, sHitBoxRect * a, int px2, int py2, sHitBoxRect * b)
	{
	   if((px2 + b->x1) + (px2 + b->x2)-(px2 + b->x1) < (px1 + a->x1))
		  return false;
	   if((px2 + b->x1) > (px1 + a->x1) + (px1 + a->x2)-(px1 + a->x1))
		  return false;

	   if((py2 + b->y1) + (py2 + b->y2)-(py2 + b->y1) < (py1 + a->y1))
		  return false;
	   if((py2 + b->y1) > (py1 + a->y1) + (py1 + a->y2)-(py1 + a->y1))
		  return false;

	   return true;
	}

	int Rand(int min, int max)
	{
		return (int) (min + ((float) rand() / RAND_MAX * (max - min )));
		//return (min + rand()%(max - min));
	}
	
	void ToggleRearPlane(bool t)
	{
		if(t == true)
			glClearColor(0, 0, 0, 0);
		else
			glClearColor(0, 0, 0, 31);
	}

	int LoadBackgroundEFS(char* filename, char* palette)
	{
		//Création du BG
		vramSetBankE(VRAM_E_MAIN_BG);
		int bg = bgInit(3, BgType_Bmp8, BgSize_B8_256x256, 0, 0);

		//Chargement
		FILE* file = fopen(filename, "rb");
		fread(bgGetGfxPtr(bg), sizeof(unsigned char), 256*256, file);
		fclose(file);

		file = fopen(palette, "rb");
		fread(BG_PALETTE, sizeof(unsigned short), 256*2, file);
		fclose(file);

		//Priorités
		bgSetPriority(bg, 3);
		bgSetPriority(0, 2);
		bgShow(3);
		bgWrapOn(bg);
		BLEND_CR = BLEND_ALPHA | BLEND_SRC_BG0 | BLEND_DST_BG3; 
		return bg;
	}

	int LoadBackgroundEFSSub(char* filename, char* palette)
	{
		//Création du BG
		vramSetBankC(VRAM_C_SUB_BG);
		int bg = bgInitSub(3, BgType_Bmp8, BgSize_B8_256x256, 0, 0);

		//Chargement
		FILE* file = fopen(filename, "rb");
		fread(bgGetGfxPtr(bg), sizeof(unsigned char), 256*256, file);
		fclose(file);

		file = fopen(palette, "rb");
		fread(BG_PALETTE_SUB, sizeof(unsigned short), 256*2, file);
		fclose(file);

		//Priorités
		/*bgSetPriority(bg, 5);
		bgSetPriority(0, 2);*/
		bgShow(3);
		bgWrapOn(bg);
		return bg;
	}

	//Sqrt hardware
	s32 hwSqrt64(int64 a)
	{
		SQRT_CR = SQRT_64;
		while(SQRT_CR & SQRT_BUSY);
		SQRT_PARAM64 = a;
		while(SQRT_CR & SQRT_BUSY);
		return SQRT_RESULT32;
	}
	
	float SquareRootFloat(float number)
	{
		long i;
		float x, y;
		const float f = 1.5F;
	    
		x = number * 0.5F;
		y  = number;
		i  = * ( long * ) &y;
		i  = 0x5f3759df - ( i >> 1 );
		y  = * ( float * ) &i;
		y  = y * ( f - ( x * y * y ) );
		y  = y * ( f - ( x * y * y ) );
		return number * y;
	}


	// Assumed to be compiled as ARM code
	/*int swi_atan2(int x, int y)
	{
		int result;

		asm volatile(
		"mov r0, %1\n"
		"mov r1, %2\n"
		"swi #0x000A0000\n"
		"mov %0, r0"
		: "=r" (result)
		: "r" (x), "r" (y)
		: "r0", "r1", "r2", "r3");
	};

	// If you are using C this might work
	// Again, ARM is expected
	int swi_atan2_c(int x, int y)
	{
		asm("swi #0x000A0000\n");
	}; */

	int GetAngle2Points(int x1, int y1, int x2, int y2)
	{
		int i = x1 - x2;
		int j = -y1 + y2;
		return atan2(i, j)*16384/3.14159265f;
	}
}
