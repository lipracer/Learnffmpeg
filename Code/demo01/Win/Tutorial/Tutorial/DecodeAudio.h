#ifndef _DECODEAUDIO_H_
#define _DECODEAUDIO_H_

#include <Windows.h>

extern "C"
{
#include "libavcodec\avcodec.h"
#include "libavformat\avformat.h"
#include "libswscale\swscale.h"
}

class DecodeAudio
{
public:
	DecodeAudio();
	~DecodeAudio();

	bool init_audio_device(char* block, int size);

	int decode_audio();

	HWAVEOUT m_hWaveOut;
};

#endif // _DECODEAUDIO_H_