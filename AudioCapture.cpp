#include "AudioCapture.h"
#include <iostream>

static int callback(const void *inputBuffer, void *outputBuffer,
	unsigned long framesPerBuffer,
	const PaStreamCallbackTimeInfo* timeInfo,
	PaStreamCallbackFlags statusFlags,
	void *userData)
{
	auto ac = (AudioCapture*)userData;
	for (int c = 0; c < framesPerBuffer; c++)
		ac->waveform[c].r = ((float*)inputBuffer)[c];
	ac->amplitudeRecalc = 1;
	
	kiss_fft(ac->cfg, ac->waveform, ac->fft);

	return paContinue;
}

AudioCapture::AudioCapture()
{
	waveform = new kiss_fft_cpx[bufferSize];
	fft = new kiss_fft_cpx[bufferSize];
	cfg = kiss_fft_alloc(bufferSize, 0, NULL, NULL);

	for (int c = 0; c < bufferSize; c++)
		waveform[c].i = 0;

	Pa_Initialize();
	PaStreamParameters inputParameters;
	auto n = Pa_GetDefaultInputDevice();
	auto info = Pa_GetDeviceInfo(n);
	inputParameters.channelCount = info->maxInputChannels;
	inputParameters.device = n;
	inputParameters.sampleFormat = paFloat32;
	inputParameters.suggestedLatency = info->defaultLowInputLatency;
	inputParameters.hostApiSpecificStreamInfo = NULL;
	Pa_OpenStream(&stream, &inputParameters, NULL, 44100, bufferSize, paNoFlag, callback, (void*)this);
	Pa_StartStream(stream);
}


AudioCapture::~AudioCapture()
{
	Pa_Terminate();
	delete[] waveform;
	delete[] fft;
	free(cfg);
}

void AudioCapture::update()
{
	
}

float AudioCapture::getAmplitude()
{
	static float sum;
	if (amplitudeRecalc)
	{
		amplitudeRecalc = 0;
		sum = 0;
		for (int c = 0; c < bufferSize; c += bufferSize / 10)
		{
			sum += abs(waveform[c].r);
		}
		sum /= 10;
		sum *= boost;
		return sum;
	}
	return sum;
}
