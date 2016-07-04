#pragma once
#include "bass.h"
#include <cmath>
class AudioCapture
{
	
	float *waveform, *FFT;
	DWORD recordHandle;
	int boost = 10000;
	const int bufferSize = 1024;
	bool amplitudeRecalc = 1;
public:
	AudioCapture();
	~AudioCapture();
	
	void update();
	float getWaveform(float i){ return boost * waveform[(int)(i*bufferSize)]; }
	float getFFT(float i){ return 10 * boost * FFT[(int)(i*bufferSize/2)]; }
	float getAmplitude();
};

