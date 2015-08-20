/* --------------------------- */
/* Natura no Story             */
/* --------------------------- */

#ifndef H_SOUNDMANAGER
#define H_SOUNDMANAGER

//Includes
#include "../Platform.h"
#include "../system/singleton.h"
#include "ima_adpcm.h"
#include <string>
#include <vector>
#include <list>
#include <maxmod9.h>
#include "../soundbank.h"  // Soundbank definitions

//Structures
struct sMusic
{
	std::string name;
};

struct sSoundData
{
	u8* buffer;
	int size;
	mm_ds_sample* sample;
};

struct sSound
{
	int soundID;
	sSoundData* data;
	int type;
	int posx;
};

//Sons
#define SND_MENU           0
#define SND_SELECT         1
#define SND_BACK           2
#define SND_JUMP           3
#define SND_SBR1           4
#define SND_SBR2           5
#define SND_LASER          6
#define SND_WOOD           7
#define SND_DISAP          8
#define SND_EXPLODE        9
#define SND_LIQUID         10

//Classe
namespace LowLevel
{

class SoundManager : public Singleton<SoundManager>
{
	friend class Singleton<SoundManager>;
	public:
		SoundManager();
		~SoundManager();
		void Init();

		//Musique
		void PlayMusic(std::string filename, int playmode);
		void TogglePauseMusic();
		void RestartMusic();
		void StopMusic();

		//Effets sonores
		void LoadSound(std::string filename);
		void PlaySoundSimple(int sound);
		void PlaySoundWorld(int sound, int posx); //Position x en world coordinates
		//Gestion
		void Handle();

	public:
		//Données internes
		sMusic music;
		IMA_Adpcm_Player player;
		bool isRequested;
		int starter;
		int nextfreechannel;
};

};
#endif