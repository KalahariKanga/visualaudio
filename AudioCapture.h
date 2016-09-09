#pragma once
#include "portaudio.h"
#include "kiss_fft.h"
#include <cmath>
class AudioCapture
{
	int boost = 5000;
	const int bufferSize = 512;
	int waveformSize;
	bool amplitudeRecalc = 1;

	PaStream* stream;

	kiss_fft_cfg cfg;
	kiss_fft_cpx *waveform, *fft, *data;
public:
	AudioCapture();
	~AudioCapture();
	
	void update();
	float getWaveform(float i);
	float getFFT(float i);
	float getFFT(float a, float b);
	float getAmplitude();
	
	void normalise();

	friend static int callback(const void *inputBuffer, void *outputBuffer,
		unsigned long framesPerBuffer,
		const PaStreamCallbackTimeInfo* timeInfo,
		PaStreamCallbackFlags statusFlags,
		void *userData);

};

