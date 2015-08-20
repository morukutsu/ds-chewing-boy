/* --------------------------- */
/* Natura no Story             */
/* --------------------------- */

#ifndef H_PLATFORM_NDS
#define H_PLATFORM_NDS

//Includes
#include <vector>
#include <fat.h> 
#include "efs_lib.h"
#include <maxmod9.h>
//#include "as_lib9.h"
#include "FixedPoint.h"
#include "3DWrapper.h"
#include "Image.h"
#include "Drawable.h"
#include "Sprite.h"
#include "Sub2D.h"
#include "Map.h"
#include "Model.h"
#include "Text.h"
#include "InputManager.h"
#include "SoundManager.h"
#include "ParticleGenerator.h"
#include <nds/registers_alt.h>
#include <nds.h>
#include "../System/singleton.h"
#include "Backbuffer.h"
#include <math.h>
//#include <PA9.h>
#include "ideas.h"
#include "HitBoxes.h"


//Paramètres statiques de la plateforme cible
#define P_SCREENW            256        //Largeur écran
#define P_SCREENH            192        //Hauteur écran
#define P_FPS                 60        //Frames par secondes
#define P_IPS                 60        //Itérations par secondes

//Structures
int getMemUsed();
int getMemFree();

extern "C" int N3DNoCashMsg(const char *pText);
int N3DNoCashMsg(const char *pText);

#define NOGBA(_fmt, _args...) do { char nogba_buffer[256]; sprintf(nogba_buffer, _fmt, ##_args); N3DNoCashMsg(nogba_buffer); } while(0)

void HBLFunc();
void VBLFunc();

#define SWIatanDEC(x,y) (\
{\
s16 __value     ;\
s16 __arg1 = (x);\
s16 __arg2 = (y);\
__asm volatile (\
   " mov r0,%1  \n\
     mov r1,%2  \n\
     swi 0xa0000 \n\
     mov %0,r0"\
     : "=r" (__value) \
     : "r" (__arg1)   \
     , "r" (__arg2)   \
     : "r0", "r1");\
(__value/180);\
}) 

namespace LowLevel
{
	//NDS
	//extern u8 soundData[784*1024];  //Buffer son : 784kb (0,8mo)

    bool uStylusInZone(u16 x1, u16 y1, u16 x2, u16 y2);
    bool RectCollision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);
	bool RectCollision(int px1, int py1, sHitBoxRect * a, int px2, int py2, sHitBoxRect * b);
    bool RectPointCollision(int x1, int y1, int x2, int y2, int w2, int h2);
	bool RectPointCollision(int x1, int y1, int x2, int y2, sHitBoxRect * a);
	bool AABBCollide(int px1, int py1, sHitBoxRect* a, int px2, int py2, sHitBoxRect* b);
	s32 hwSqrt64(int64 a);
	int GetAngle2Points(int x1, int y1, int x2, int y2);
	float SquareRootFloat(float number);

	int Rand(int min, int max);
	void ToggleRearPlane(bool t);
	int LoadBackgroundEFS(char* file, char* palette);
	int LoadBackgroundEFSSub(char* filename, char* palette);

    class Platform : public Singleton<Platform>
    {
	friend class Singleton<Platform>;
    public:
        Platform();
        void Init();
        void Unload();
        void BeginDisplay();
        void EndDisplay();
        void Sync();
		
		void ActiveEffectSineWave(bool t, int speed, int mult, int intensity);

    private:
        static bool isInited;

	public:
		//Variables des fonctions HBL/VBL
		static int hbladd;
		static int sineWaveMult, sineWaveIntensity, sineWaveSpeed;
		static bool sineWaveActivated;
		static OamState *oam;
		static int paletteOffset;
    };
}

#endif
