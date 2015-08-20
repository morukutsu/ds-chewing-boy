/* --------------------------- */
/* Inside the machine 2        */
/* --------------------------- */

#include "ParticleGenerator.h"

sGenPreset genPresets[14] = { 
	{0, -256, PART_TEX_BALL, GENERATOR_SHORT, 5, 32767, RGB15(31,31,31), 8, 32, 256, 32, 8, 2, END_METHOD_FADE, 16},
	{0, -256, PART_TEX_ROCK, GENERATOR_SHORT, 4, RGB15(31,31,31), RGB15(31,31,31), 8, 32, 256, 128, 8, 2, END_METHOD_SCALEFADE, 16},
	{0, 0, PART_TEX_EPINES, GENERATOR_SHORT, 6, RGB15(31,31,31), RGB15(31,31,31), 8, 32, 256, 256, 8, 2, END_METHOD_SCALEFADE, 0},
	{0, -64, PART_TEX_SMOKE, GENERATOR_SHORT, 4, RGB15(31,31,31), RGB15(31,31,31), 8, 26, 256, 0, 8, 2, END_METHOD_SCALEFADE, 0},
	{0, 0, PART_TEX_SMOKE, GENERATOR_SHORT, 16, RGB15(0,0,0), RGB15(5,5,5), 8, 26, 256, 256, 8, 2, END_METHOD_SCALEFADE, 0},
	{0, -32, PART_TEX_SMOKE, GENERATOR_SHORT, 6, RGB15(31,31,31), RGB15(31,31,31), 12, 16, 64, 64, 2, 2, END_METHOD_FADE, 0},
	{0, 32, PART_TEX_SMOKE, GENERATOR_SHORT, 8, RGB15(31,31,31), RGB15(31,31,31), 12, 32, 16, 96, 2, 2, END_METHOD_FADE, 0},
	{0, -32, PART_TEX_SMOKE, GENERATOR_SHORT, 8, RGB15(31, 31, 31), RGB15(30, 25, 30), 12, 32, 128, 8, 2, 2, END_METHOD_FADE, 0},
	{0, 0, PART_TEX_SMOKE, GENERATOR_SHORT, 16, RGB15(31, 31, 31), RGB15(31, 31, 31), 32, 32, 96, 128, 2, 2, END_METHOD_FADE, 0},
	{0, 0, PART_TEX_FLARE, GENERATOR_SHORT, 16, RGB15(31, 31, 31), RGB15(31, 31, 31), 32, 32, 96, 128, 2, 2, END_METHOD_FADE, 0},
	{0, 0, PART_TEX_EPINES2, GENERATOR_SHORT, 3, RGB15(31,31,31), RGB15(31,31,31), 8, 32, 128, 128, 8, 2, END_METHOD_SCALEFADE, 0},
	{0, 0, PART_TEX_EPINES2, GENERATOR_SHORT, 4, RGB15(15,15,31), RGB15(31,31,31), 8, 32, 256, 256, 8, 2, END_METHOD_SCALEFADE, 0},
	{0, -32, PART_TEX_SMOKE, GENERATOR_SHORT, 4, RGB15(31, 31, 31), RGB15(31, 31, 31), 10, 32, 128, 8, 2, 2, END_METHOD_FADE, 0},
	{0, -384, PART_TEX_FLARE, GENERATOR_CONSTANT, 16, RGB15(31, 31, 31), RGB15(31, 31, 31), 16, 64, 64, 8, 2, 2, END_METHOD_FADE, 0}
	};

namespace LowLevel
{
	ParticleGenerator::ParticleGenerator()
	{
		
	}
	
	void ParticleGenerator::Init()
	{
		mNextHit = mNextGen = 0;
		mNextFreeHit = mNextFreeGen = -1;
		mHitCount = mGenCount = 0;

		//Preload particles
		System::CRessourceManager::getInstance()->LoadImage("efs:/data/sprites/Particle_Ball.png", UL_IN_VRAM, UL_PF_PAL4);
		System::CRessourceManager::getInstance()->LoadImage("efs:/data/sprites/Particle_Smoke.png", UL_IN_VRAM, UL_PF_PAL4);
		System::CRessourceManager::getInstance()->LoadImage("efs:/data/sprites/Particle_Bulles.png", UL_IN_VRAM, UL_PF_PAL4);
		System::CRessourceManager::getInstance()->LoadImage("efs:/data/sprites/Particle_Rock.png", UL_IN_VRAM, UL_PF_PAL4);
		System::CRessourceManager::getInstance()->LoadImage("efs:/data/sprites/Particle_Epines.png", UL_IN_VRAM, UL_PF_PAL2);
		System::CRessourceManager::getInstance()->LoadImage("efs:/data/sprites/Particle_Epines2.png", UL_IN_VRAM, UL_PF_PAL2);
		System::CRessourceManager::getInstance()->LoadImage("efs:/data/sprites/spr_flare_1.png", UL_IN_VRAM, UL_PF_PAL3_A5); 
	}

	int ParticleGenerator::AddHit(int type, int x, int y)
	{
		int hitID;

		return hitID;
	}

	int ParticleGenerator::AddGenerator(int type, int x, int y)
	{
		int genID = 0;
		if(mNextGen == MAXGENS)
			mNextGen = 0;
		if(mGenCount < MAXGENS)
		{
			//Calcul de l'ID
			if(mNextFreeGen != -1)
			{
				genID = mNextFreeGen;
				mNextFreeGen = -1;
			}
			else
			{
				genID = mNextGen;
				mNextGen++;
			}

			//Création du génétateur
			mGens[genID].Set(genPresets[type]);
			mGens[genID].x = x;
			mGens[genID].y = y;
			mGens[genID].free = false;
			mGens[genID].worldMan = worldMan;
			mGens[genID].Load();
			mGenCount++;
		}
		/*else
			ulDebug("Gen Overload\n");*/
		return genID;
	}

	void ParticleGenerator::DrawGens()
	{
		for(int k = 0; k < MAXGENS; k++)
		{
			if(!mGens[k].free)
			{
				if(mGens[k].toDelete)
				{
					mNextFreeGen = k;
					mGens[k].free = true;
					mGenCount--;
				}
				else
				{
					mGens[k].Update();
					mGens[k].Draw();
				}
			}
		}
	}


};