#pragma once
#include "portaudio.h"
#include <cmath>
class AudioCapture
{
	
	float *waveform, *FFT;
	
	int boost = 5000;
	const int bufferSize = 1024;
	bool amplitudeRecalc = 1;

	PaStream* stream;
public:
	AudioCapture();
	~AudioCapture();
	
	void update();
	float getWaveform(float i){ return boost * waveform[(int)(i*bufferSize)]; }
	float getFFT(float i){ return 10 * boost * FFT[(int)(i*bufferSize/2)]; }
	float getAmplitude();
	
	friend static int callback(const void *inputBuffer, void *outputBuffer,
		unsigned long framesPerBuffer,
		const PaStreamCallbackTimeInfo* timeInfo,
		PaStreamCallbackFlags statusFlags,
		void *userData);

};

