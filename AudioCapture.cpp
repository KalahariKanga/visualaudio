#include "AudioCapture.h"
#include <iostream>

static int callback(const void *inputBuffer, void *outputBuffer,
	unsigned long framesPerBuffer,
	const PaStreamCallbackTimeInfo* timeInfo,
	PaStreamCallbackFlags statusFlags,
	void *userData)
{
	for (int c = 0; c < framesPerBuffer; c++)
		((AudioCapture*)userData)->waveform[c] = ((float*)inputBuffer)[c];
	((AudioCapture*)userData)->amplitudeRecalc = 1;
	return paContinue;
}

AudioCapture::AudioCapture()
{
	Pa_Initialize();
	PaStreamParameters inputParameters;
	auto n = Pa_GetDefaultInputDevice();
	auto info = Pa_GetDeviceInfo(n);
	inputParameters.channelCount = info->maxInputChannels;
	inputParameters.device = n;
	inputParameters.sampleFormat = paFloat32;
	inputParameters.suggestedLatency = info->defaultLowInputLatency;
	waveform = new float[bufferSize];
	inputParameters.hostApiSpecificStreamInfo = NULL;
	Pa_OpenStream(&stream, &inputParameters, NULL, 44100, bufferSize, paNoFlag, callback, (void*)this);
	Pa_StartStream(stream);
}


AudioCapture::~AudioCapture()
{
	Pa_Terminate();
	delete[] waveform;
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
			sum += abs(waveform[c]);
		}
		sum /= 10;
		sum *= boost;
		return sum;
	}
	return sum;
}