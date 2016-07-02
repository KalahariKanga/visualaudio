#pragma once
#include "bass.h"

class AudioCapture
{
	const int bufferSize = 1024;
	float* waveform, FFT;
	DWORD recordHandle;
public:
	AudioCapture();
	~AudioCapture();
	void update();
	float* getWaveform(){ return waveform; }
};

