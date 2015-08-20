#ifndef _DS_IMA_ADPCM_H
#define _DS_IMA_ADPCM_H

#include <stdio.h>

enum {
	IMA_ADPCM_OKAY,
	IMA_ADPCM_ERROR_CANNOTOPEN,
	IMA_ADPCM_ERROR_NOTRIFFWAVE,
	IMA_ADPCM_ERROR_UNSUPPORTED,
	IMA_ADPCM_ERROR_INVALIDCHANNELS
};

//**********************************************************************************************
//**********************************************************************************************
//**********************************************************************************************

typedef struct t_IMA_Adpcm_Data {

	int curSamps;
	int data1, step1, samp1;
	int data2, step2, samp2;
} IMA_Adpcm_Data;

//**********************************************************************************************
//**********************************************************************************************
//**********************************************************************************************

class IMA_Adpcm_Stream
{
private:
	IMA_Adpcm_Data		data;
	IMA_Adpcm_Data		loop_data;
	
	u8*		datacache;

	u8*		loop_src;
	
	int		loop_cblock;
	int		loop_state;
	int		loop_br;
	int		loop_oddnibble;
	
	int		wave_data;
	int		wave_end;
	u8		*srcb;
	u8		*src;

	
	int		oddnibble;
	int		block;
	int		blockremain;
	int		loop1, loop2;
	int		channels;
	int		position;
	int		currentblock;
	int		state;
	int		format;
	int		sampBits;
	int		sampling_rate;
	int		wave_loop;
	
	FILE	*fin;
	int		is_processing;
	
	void	open( const void* );
	void	skip( int );
	void	seek( int );
	int		tell();
	int		fget8();
	int		fget16();
	u32		fget32();
	void	getblock();
	int		get8();
	int		get16();
	u32		get32();
	
	void	capture_frame();
	void	restore_frame();

	int		stream_pcm( s16 *target, int length );
	int		decode_ima( s16 *target, int length );

	
public:

	IMA_Adpcm_Stream();
	~IMA_Adpcm_Stream();
	
	int reset( const char *wav_file, bool loop );
	int stream( s16 *target, int length );
	void close();
	
	int get_channels();
	int get_sampling_rate();
	int get_mm_format();
};

//**********************************************************************************************
//**********************************************************************************************
//**********************************************************************************************

class IMA_Adpcm_Player {

	IMA_Adpcm_Stream stream;
	int volume;
	bool autofill;
	bool paused;
	bool active;
	
public:
	IMA_Adpcm_Player();
	int play( const char *file, bool loop_audio, bool automatic_updates, int buffer_length = 2048 );
	void pause();
	void resume();
	void stop();
	void setvolume( int vol );
	int getvolume();
	bool isactive();
	bool ispaused();
	mm_word i_stream_request( mm_word length, mm_addr dest, mm_stream_formats format );
	
	// update function for manual filling
	void update();
};

#endif
