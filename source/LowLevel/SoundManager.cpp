/* --------------------------- */
/* Natura no Story             */
/* --------------------------- */

//Includes
#include "SoundManager.h"

namespace LowLevel
{

SoundManager::SoundManager()
{
	isRequested = false;
	starter = -1;
	soundEnable();

	//On charge tout les sons
	/*LoadSound("efs:/sounds/Snd_Menu.raw");
	LoadSound("efs:/sounds/Snd_Select.raw");
	LoadSound("efs:/sounds/Snd_Back.raw");
	LoadSound("efs:/sounds/Snd_Jump.raw");
	
	*/
	mmLoadEffect(SFX_SND_MENU );
	mmLoadEffect(SFX_SND_SELECT );
	mmLoadEffect(SFX_SND_BACK );
	mmLoadEffect(SFX_SND_JUMP );
	mmLoadEffect(SFX_SND_SBR1 );
	mmLoadEffect(SFX_SND_SBR2 );
	mmLoadEffect(SFX_SND_LASER );
	mmLoadEffect(SFX_SND_LIQUID );
	mmLoadEffect(SFX_SND_EXPLODE );
	mmLoadEffect(SFX_SND_WOOD );
	mmLoadEffect(SFX_SND_DISAP );
}

SoundManager::~SoundManager()
{

}

void SoundManager::Init()
{

}


void SoundManager::PlayMusic(std::string filename, int playmode)
{
	//On démarre la lecture en streaming
	music.name = filename;
	isRequested = true;
	starter = 0;
	//soundDisable();
	player.play(filename.c_str(), true, true, 8192);
}

void SoundManager::TogglePauseMusic()
{
	player.pause();
}

void SoundManager::RestartMusic()
{
	player.resume();
}

void SoundManager::StopMusic()
{
	//STREAM_Stop();
	player.stop();
}

void SoundManager::LoadSound(std::string filename)
{
	//Chargemment d'un son depuis l'efs
	//On vide le buffer mémoire (spéficique a ITM attention)
	//*memset(soundData, 0, 1024*1024);*/
	
	//On définit quelques variables
    /*FILE* file;
	sSoundData tmp;

	// allocate memory for sample
    // the sample header and data must be in mainram (for ARM7 side)
    tmp.sample = (mm_ds_sample*)malloc( sizeof( mm_ds_sample ) );
   
    // disable loop
    tmp.sample->loop_start = 0;
   
    // format: 0 = 8bit, 1 = 16bit, 2 = adpcm
    tmp.sample->format = 0;
   
    // repeat, 1 = looping sample, 2 = one-shot sample
    tmp.sample->repeat_mode = 2;
   
    // base rate, 0x400/2 = 16KHz, (KHz = (base_rate * 32) / 0x400)
    //tmp.sample->base_rate = (11025*0x400)/32;
	tmp.sample->base_rate = 0x400/2;
   
    //On ouvre le fichier
    file = fopen(filename.c_str(), "rb");
   
    if(file != NULL)
    {
	    //Cn charge le fichier en mémoire
	    fseek(file, 0, SEEK_END);
	    int size = ftell(file); 
	    fseek(file, 0, SEEK_SET);
		
		tmp.size = size;

		// set length = 1000 words (4000 samples @ 8bit)
		tmp.sample->length = size;
		
		// allocate sample data
		tmp.sample->data = malloc(tmp.sample->length * 4);
		
	   
	    //On alloue de la mémoire
	    int result = fread(tmp.sample->data, 1, size, file);
	    fclose(file);
		snddata.push_back(tmp);
	}*/
}

void SoundManager::PlaySoundSimple(int sound)
{
	//On lit le son
	//soundPlaySample(snddata[sound].buffer, SoundFormat_8Bit, snddata[sound].size, 11025, 127, 64, false, 0);
	/*mm_sound_effect effect;
   
    // sample = sample address of our sample
    // the 'id' parameter is not used
    effect.sample = snddata[sound].sample;
   
    // rate = 0x400 which is 1.0 which is original playback rate
    effect.rate = 0x400;
   
    // handle = 0 means allocate a new handle
    effect.handle = 0;
   
    // volume = 200/256 = ~78%
    effect.volume = 255;
   
    // panning = 128/256 = centered
    effect.panning = 128;
   
    // play sound
    mmEffectEx( &effect );*/

	if(sound == SND_MENU)
		mmEffect( SFX_SND_MENU );
	if(sound == SND_SELECT)
		mmEffect( SFX_SND_SELECT );
	if(sound == SND_BACK)
		mmEffect( SFX_SND_BACK );
	if(sound == SND_JUMP)
		mmEffect( SFX_SND_JUMP );
	if(sound == SND_WOOD)
		mmEffect( SFX_SND_WOOD );
	if(sound == SND_DISAP)
		mmEffect( SFX_SND_DISAP );
	if(sound == SND_EXPLODE)
		mmEffect( SFX_SND_EXPLODE );
	if(sound == SND_LIQUID)
		mmEffect( SFX_SND_LIQUID );
	if(sound == SND_LASER)
		mmEffect( SFX_SND_LASER );
	if(sound == SND_SBR1)
		mmEffect( SFX_SND_SBR1 );
	if(sound == SND_SBR2)
		mmEffect( SFX_SND_SBR2 );

}

void SoundManager::PlaySoundWorld(int sound, int posx)
{

}

void SoundManager::Handle()
{
	//Gestion des requêtes de lecture
	if(isRequested == true)
	{
		//STREAM_PlayFile((char*)music.name.c_str(), false );
		isRequested = false;
	}
	if(starter != -1)
	{
		starter++;
	}
	if(starter == 5)
	{
		//setvolume(256);
		//soundEnable();
	}

	//mmStreamUpdate();
	//player.update();
}

};