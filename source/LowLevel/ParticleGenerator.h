/* --------------------------- */
/* Inside the machine 2        */
/* --------------------------- */

#ifndef H_PARTICLEGENERATOR
#define H_PARTICLEGENERATOR

#include "fxHit.h"
#include "fxGenerator.h"
#include "../System/singleton.h"
#include "../System/RessourceManager.h"

#define PARTICLE_HIT        0
#define PARTICLE_GEN        1
#define MAXHITS             32
#define MAXGENS				16

class WorldManagerOffline;
namespace LowLevel
{
	//Générateur de particules
	class ParticleGenerator : public Singleton<ParticleGenerator>
	{
		friend class Singleton<ParticleGenerator>;
		
		public:
			ParticleGenerator();
			void Init();
			int AddHit(int type, int x, int y);
			int AddGenerator(int type, int x, int y);

			void DrawGens();

		public:
			WorldManagerOffline *worldMan;
			//Tableaux de générateurs
			fxHit mHits[MAXHITS];
			fxGenerator mGens[MAXGENS];
			int mNextHit, mNextFreeHit;
			int mNextGen, mNextFreeGen;	
			int mHitCount, mGenCount;
	};
};

#endif