#pragma once
#include "bass.h"

class AudioCapture
{
	
	float* waveform, FFT;
	DWORD recordHandle;
	int boost = 10000;
	const int bufferSize = 1024;
public:
	AudioCapture();
	~AudioCapture();
	
	void update();
	float getWaveform(float i){ return boost * waveform[(int)(i*bufferSize)]; }
	float getAmplitude();
};

