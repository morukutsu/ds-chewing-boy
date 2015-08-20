/***************************
 * ima-adpcm decoder by Discostew
 * base decoder/player template by mukunda
 ***************************/
#include <nds.h>
#include <maxmod9.h>

#include "ima_adpcm.h"


#define seek(x) fseek( fin, (x), SEEK_SET )
#define skip(x) fseek( fin, (x), SEEK_CUR )
#define tell() ftell(fin)

extern "C" {
	void decode_mono_ima_adpcm( IMA_Adpcm_Data *data, u8 *src, s16 *dest, int iterations );
	void decode_stereo_ima_adpcm( IMA_Adpcm_Data *data, u8 *src, s16 *dest, int iterations );
}

enum
{
	state_beginblock,
	state_decode
};

enum
{	
	WAV_FORMAT_PCM			= 0x1,
	WAV_FORMAT_IMA_ADPCM	= 0x11
};

IMA_Adpcm_Stream::IMA_Adpcm_Stream()
{
	fin = 0;
	datacache = 0;
}

IMA_Adpcm_Stream::~IMA_Adpcm_Stream()
{
	if( datacache )
		delete[] datacache;
}

int IMA_Adpcm_Stream::reset( const char *wav_file, bool loop )
{
	if( fin ) {
		close();
	}
	fin = fopen(wav_file, "rb");
	
	if( !fin )
		return IMA_ADPCM_ERROR_CANNOTOPEN;

	if( fget32() != 0x46464952 )		// "RIFF"
	{
		fclose( fin );
		return IMA_ADPCM_ERROR_NOTRIFFWAVE;
	}
	int size = fget32();
	if( fget32() != 0x45564157 )		// "WAVE"
	{
		fclose( fin );
		return IMA_ADPCM_ERROR_NOTRIFFWAVE;
	}
	
	// parse WAV structure
	while( tell() < size )
	{
		u32 code = fget32();
		u32 csize = fget32();
		switch( code )
		{
		case 0x20746D66: 	// format chunk
			
			// catch invalid format
			format = fget16();
			if(( format != WAV_FORMAT_PCM ) && ( format != WAV_FORMAT_IMA_ADPCM ))
			{
				fclose( fin );
				return IMA_ADPCM_ERROR_UNSUPPORTED;
			}
			
			channels = fget16();
			
			// catch invalid channels
			if(( channels < 1 ) || ( channels > 2 ))
			{
				fclose( fin );
				return IMA_ADPCM_ERROR_INVALIDCHANNELS;
			}
				
			sampling_rate = fget32();// sample rate
			skip( 4 );	// avg bytes/second
			
			block = fget16();
			
			datacache = new u8[block];

			sampBits = fget16();

			if((( format == WAV_FORMAT_PCM ) && (( sampBits != 8 ) && ( sampBits != 16 ))) ||
				(( format == WAV_FORMAT_IMA_ADPCM ) && ( sampBits != 4 )))
			{
				fclose( fin );
				return IMA_ADPCM_ERROR_UNSUPPORTED;
			}
			
			skip( csize - 0x10 );

			break;
			
		case 0x61746164:	// data chunk
			wave_data = tell();
			loop1 = 0;
			skip( csize );
			wave_end = tell();
			if( format == WAV_FORMAT_PCM )
				loop2 = csize >> ( sampBits == 16 ? channels : ( channels - 1 ));
			else
			{
				//printf("chan:%d\n", channels);
				if(channels == 2)
					loop2 = csize << ( 2 - channels );
				if(channels == 1)
					loop2 = ((csize << ( 2 - channels ))>>1)-2048;
			}
			break;
			
		case 0x6C706D73:	// sampler chunk
		{
			int s;
			skip( 28 );
			int nl = fget32();
			skip(4);
			s = 36;
			if( nl && loop) 
			{
				skip( 8 );
				loop1 = fget32() >> ( 2 - channels );
				loop2 = fget32() >> ( 2 - channels );
				s += 8+4+4;
			}
			skip( csize - s );
		}
			break;
		default:
			skip( csize );
		}
	}
	wave_loop = loop;
	oddnibble = 0;
	data.curSamps = 0;
	position = 0;
	seek( wave_data );
	currentblock = tell();
	state = state_beginblock;
	return IMA_ADPCM_OKAY;
}

int IMA_Adpcm_Stream::stream( s16 *target, int length )
{
	if( format == WAV_FORMAT_PCM )
		return stream_pcm( target, length );
	else
		return decode_ima( target, length );
}

int IMA_Adpcm_Stream::stream_pcm( s16 *target, int length )
{
	if( fin )
	{
		if( !wave_loop && ( currentblock >= wave_end ))
			return 1;
		while( length )
		{
			if( !wave_loop ) {
				if( position >= loop2 )
				{
					int i = length * channels;
					if( sampBits == 8 ) i >>= 1;
					for( ; i; i-- )
						*target++ = 0;
					length = 0;
					break;
				}
			}
			if( position == loop1 ) loop_cblock = currentblock;

			int iterations, cpysize;

			iterations = length;
			if( position < loop1 )
			{
				if( position + iterations >= loop1 )
					iterations = loop1-position;
			}
			else if( position < loop2 )
			{
				if( position + iterations >= loop2 )
					iterations = loop2-position;
			}
			cpysize = iterations << ( sampBits == 16 ? channels : ( channels - 1 ));
			fread( target, 1, cpysize, fin );
			length -= iterations;
			position += iterations;
			currentblock += cpysize;
			if( sampBits == 8 )
				target += iterations >> ( 2 - channels );
			else
				target += iterations * channels;

			
			if(( position == loop2 ) && wave_loop ) {
				seek( loop_cblock );
				currentblock = loop_cblock;
				position = loop1;
			}	
		}
	}
	return 0;
}

int IMA_Adpcm_Stream::decode_ima( s16 *target, int length )
{
	if( fin )
	{
		if( !wave_loop && ( currentblock >= wave_end ))
			return 1;
		while(length)
		{
			switch( state )
			{
				case state_beginblock:
				{
					getblock();
					if( !wave_loop ) {
						if( position >= loop2 )
						{
							int i = length * channels;
							for( ; i; i-- ) {
								*target++ = 0;
							}
							length = 0;
							break;
						}
					}
					else
					{
						if( position == loop1 ) capture_frame();
						if( position >= loop2 ) restore_frame();
					}

					data.samp1 = (short int)get16();
					data.step1 = (short int)get16();

					*target++ = data.samp1;
					if( channels == 2 )
					{
						data.samp2 = (short int)get16();
						data.step2 = (short int)get16();
						*target++ = data.samp2;
					}
										
					blockremain -= 8;
					state = state_decode;
					
					length--;
					position++;
					
					if( position == loop1 ) capture_frame();
					if( position == loop2 ) restore_frame();
					
					break;
				}
				case state_decode:
				{
					int iterations;
					
					iterations = (length > blockremain ? blockremain : length);
					
					if( position < loop1 )
					{
						if( position + iterations >= loop1 )
							iterations = loop1-position;
					}
					else if( position < loop2 )
					{
						if( position + iterations >= loop2 )
							iterations = loop2-position;
					}
					
					if( channels == 2 )
					{
						src = srcb + data.curSamps;
						decode_stereo_ima_adpcm( &data, src, target, iterations );
						srcb += iterations;
						position += iterations;
					}
					else
					{
						src = srcb + ( data.curSamps >> 1 );
						decode_mono_ima_adpcm( &data, src, target, iterations );
						srcb += ( iterations >> 1 );
						position += ( iterations >> 1 );

						if( iterations & 0x1 )
						{
								oddnibble = !oddnibble;
								if( oddnibble )
								{
									srcb++;
									position++;
								}
						}
					}
					
					length -= iterations;
					blockremain -= iterations;
					target += ( iterations * channels );
					
					if( position == loop1 ) capture_frame();
					//printf("p:%d\nl:%d\n", position, loop2);
					if( position == loop2 )
					{
						//printf("toast\n");
						restore_frame();
						break;
					}
					
					if( blockremain == 0 )
						state = state_beginblock;
					break;
				}
			}
		}
	}
	return 0;
}

void IMA_Adpcm_Stream::close() {
	if( fin )
		fclose( fin );
	fin = 0;
}

void IMA_Adpcm_Stream::capture_frame()
{
	loop_data = data;
	loop_src = srcb;
	loop_oddnibble = oddnibble;
	loop_state = state;
	loop_br = blockremain;
	loop_cblock = currentblock;
}

void IMA_Adpcm_Stream::restore_frame()
{
	seek( loop_cblock );
	getblock();
	data = loop_data;
	srcb = loop_src;
	oddnibble = loop_oddnibble;
	state = loop_state;
	blockremain = loop_br;
	position = loop1;
}

int IMA_Adpcm_Stream::fget8() {
	u8 a[1];
	fread( a, 1, 1, fin );
	return a[0];
}

int IMA_Adpcm_Stream::fget16() {
	return fget8() + (fget8() << 8);
}

u32 IMA_Adpcm_Stream::fget32() {
	return fget16() | (fget16() << 16);
}

int IMA_Adpcm_Stream::get8() {
	return *srcb++;
}

int IMA_Adpcm_Stream::get16() {
	return get8() | (get8() << 8);
}

u32 IMA_Adpcm_Stream::get32() {
	return get16() | (get16() << 16);
}

void IMA_Adpcm_Stream::getblock()
{
	currentblock = tell();
	blockremain = block << ( 2 - channels );
	fread( datacache, 1, block, fin );
	srcb = datacache;
}

int IMA_Adpcm_Stream::get_channels() {
	return channels;
}

int IMA_Adpcm_Stream::get_sampling_rate() {
	return sampling_rate;
}

int IMA_Adpcm_Stream::get_mm_format() {
	return (( format == WAV_FORMAT_PCM ? (( sampBits >> 4 ) << 1 ) : MM_STREAM_16BIT_MONO ) + ( channels - 1 ));
}

/*********************************************
 *
 * PLAYER
 *
 *********************************************/
 
static IMA_Adpcm_Player *active_player;
static mm_word on_stream_request( mm_word length, mm_addr dest, mm_stream_formats format ) {
	return active_player->i_stream_request( length, dest, format );
}
 
IMA_Adpcm_Player::IMA_Adpcm_Player() {
	setvolume( 256 );
	active=false;
}

int IMA_Adpcm_Player::play( const char *file, bool loop_audio, bool automatic_updates, int buffer_length ) {

	active = false;
	stop();
		
	autofill = automatic_updates;
	int result = stream.reset( file, loop_audio );
	if( result )
		return result;
	
	active_player = this;
	paused = false;
	active=true;
	
	// open stream
	mm_stream s;
	s.sampling_rate = stream.get_sampling_rate();
	s.buffer_length = buffer_length;
	s.callback = on_stream_request;
	s.format = stream.get_mm_format();
	s.timer = MM_TIMER0;
	s.manual = !automatic_updates;
	mmStreamOpen( &s );
	
	return 0;
}

void IMA_Adpcm_Player::pause() {
	if( active )
		paused = true;
}

void IMA_Adpcm_Player::resume() {
	if( active )
		paused = false;
}

void IMA_Adpcm_Player::stop() {
	mmStreamClose();
	stream.close();
	active=false;
}

void IMA_Adpcm_Player::setvolume( int vol ) {
	if( vol < 0 ) vol = 0;
	if( vol > 256 ) vol = 256;
	volume = vol;
}

int IMA_Adpcm_Player::getvolume() {
	return volume;
}

bool IMA_Adpcm_Player::isactive() {
	return active;
}

bool IMA_Adpcm_Player::ispaused() {
	return paused;
}

mm_word IMA_Adpcm_Player::i_stream_request( mm_word length, mm_addr dest, mm_stream_formats format ) {
	if( !paused ) {
		if( !stream.stream( (s16*)dest, length ))
		{
			// apply volume scaler
			if( volume != 256 ) {
				s16 *d = (s16*)dest;
				int i = length;
				for( ; i; i-- ) {
					*d = ((*d) * volume) >> 8; *d++;
					*d = ((*d) * volume) >> 8; *d++;
				}
			}
		}
		else
			stop();
	} else {
		s16 *d = (s16*)dest;
		int i = length * 2;
		for( ; i; i-- ) {
			*d++ = 0;
		}
	}
	return length;
}

void IMA_Adpcm_Player::update() {
	if( active && !autofill ) {
		mmStreamUpdate();
	}
}
