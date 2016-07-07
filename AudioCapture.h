#pragma once
#include "portaudio.h"
#include "kiss_fft.h"
#include <cmath>
class AudioCapture
{
	int boost = 5000;
	const int bufferSize = 512;
	bool amplitudeRecalc = 1;

	PaStream* stream;

	kiss_fft_cfg cfg;
	kiss_fft_cpx *waveform, *fft;
public:
	AudioCapture();
	~AudioCapture();
	
	void update();
	float getWaveform(float i){ return boost * waveform[(int)(i*bufferSize)].r; }
	float getFFT(float i){ return fft[(int)(i*bufferSize/4)].r; }
	float getAmplitude();
	
	friend static int callback(const void *inputBuffer, void *outputBuffer,
		unsigned long framesPerBuffer,
		const PaStreamCallbackTimeInfo* timeInfo,
		PaStreamCallbackFlags statusFlags,
		void *userData);

};

