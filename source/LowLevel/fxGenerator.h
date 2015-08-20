/* --------------------------- */
/* Inside the machine 2        */
/* --------------------------- */

#ifndef H_FXGEN
#define H_FXGEN

#include "Sprite.h"

//Defines
#define MAX_PARTICULES         32
	//Mode
#define GENERATOR_SHORT         0
#define GENERATOR_CONSTANT      1
	//Textures
#define PART_TEX_BALL           0
#define PART_TEX_SMOKE          1
#define PART_TEX_BULLES         2
#define PART_TEX_ROCK           3
#define PART_TEX_EPINES         4
#define PART_TEX_FLARE			5
#define PART_TEX_EPINES2        6
	//End Methods
#define END_METHOD_SCALE        0
#define END_METHOD_FADE         1
#define END_METHOD_SCALEFADE    2
	//Presets
#define PARTICLES_SHORT_EAU					0
#define PARTICLES_SHORT_ROCKEXPLODE			1
#define PARTICLES_SHORT_EPINESEXPLODE		2
#define PARTICLES_SHORT_SMOKE				3
#define PARTICLES_SHORT_BOMBEXPLODE			4
#define PARTICLES_SHORT_JSMOKE				5
#define PARTICLES_SHORT_WJSMOKE				6
#define PARTICLES_SHORT_SHOTSMOKE			7
#define PARTICLES_SHORT_DISPSMOKE			8
#define PARTICLES_SHORT_FLARE				9
#define PARTICLES_SHORT_EPINESEXPLODELOW	10
#define PARTICLES_SHORT_GEMGET          	11
#define PARTICLES_SHORT_RUNSMOKE          	12
#define PARTICLES_SHORT_FAIRYESC          	13

struct sGenPreset
{
	int vx, vy, texture, mode, number, couleur, couleur2, taille, fade, range_x, range_y, boot_freq, imp_freq, end_method, gravity;
};

class WorldManagerOffline;
namespace LowLevel
{
	//Structures
	struct esParticule
	{
		bool actif;   //Définit si la particule est active
		s32 x, y;     //Position en x et y de la particule dans le monde (fixpt)
		s16 vx, vy;   //Vecteur vitesse de la particule (fixpt)
		s32 life;     //Vie de la particule
		u32 fade;     //Vitesse de disparition de la particule
		int color;    //Couleur de la particule
		int polyg;
	};

	class fxGenerator
	{
	public:
		fxGenerator();
   		void Draw();
		void Update();
		void Load();
		void Reset();
		void Set(int vx, int vy, int texture, int mode, int number, int couleur, int couleur2, int taille, int fade, int range_x, int range_y, int boot_freq, int imp_freq, int end_method, int gravity);
		void Set(sGenPreset preset);
	public:
		esParticule particules[MAX_PARTICULES];  //Tableau de particules
		int vx, vy;   						     //Vecteur vitesse du generateur
		int g;									 //Gravité
		int texture;  							 //Texture
		int number;   							 //Nombre de particules
		int color, color2;						 //Couleurs possibles des particules
		int taille;   							 //Taille
		int fade;     							 //Vitesse de disparition de la particule
		int rangex, rangey;						 //Rayon de dispertion de la particule
		int bootFreq, impFreq;					 //Fréquences d'apparition ( démarrage et impuretés )
		int mode;                                //Mode de fonctionnement du générateur
		int endMethod;							 //Methode de disparition de la particule
		int life;                                //Vie max d'une particule
		LowLevel::Sprite img;                    //Texture du générateur
		int curBootFreq, curImpFreq;             //Fréquences courantes
		bool isNewPartAccepted;                  //Définit si on peut accepter de new particules sur cette frame
		int x_screen, y_screen;                  //Position du générateur sur l'écran
		int total_number;
		int polygroupswaping;
		
		WorldManagerOffline* worldMan;
		int type, genre;
		int x, y, w, h;
		bool active;
		bool toDelete;
		bool free;
		bool stopGen;
	};
}

#endif