#include "AudioCapture.h"
#include <iostream>

static int callback(const void *inputBuffer, void *outputBuffer,
	unsigned long framesPerBuffer,
	const PaStreamCallbackTimeInfo* timeInfo,
	PaStreamCallbackFlags statusFlags,
	void *userData)
{
	auto ac = (AudioCapture*)userData;
	for (unsigned c = 0; c < framesPerBuffer; c+=2)///////////////////////TODO:ASSUMES STEREO
	{
		float a = ((float*)inputBuffer)[c];
		float b = ((float*)inputBuffer)[c + 1];
		ac->data[c].r = a;
		ac->data[c + 1].r = b;
		ac->waveform[c / 2].r = (a + b) / 2.0;
	}
	ac->amplitudeRecalc = 1;
	
	kiss_fft(ac->cfg, ac->waveform, ac->fft);

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
	inputParameters.hostApiSpecificStreamInfo = NULL;
	Pa_OpenStream(&stream, &inputParameters, NULL, 44100, bufferSize, paNoFlag, callback, (void*)this);
	Pa_StartStream(stream);

	waveformSize = bufferSize / inputParameters.channelCount;

	data = new kiss_fft_cpx[bufferSize];
	fft = new kiss_fft_cpx[bufferSize];
	waveform = new kiss_fft_cpx[waveformSize];
	cfg = kiss_fft_alloc(waveformSize, 0, NULL, NULL);

	for (int c = 0; c < bufferSize; c++)
	{
		data[c].i = 0;
	}
	for (int c = 0; c < waveformSize; c++)
	{
		waveform[c].i = 0;
	}
}


AudioCapture::~AudioCapture()
{
	Pa_Terminate();
	delete[] waveform;
	delete[] fft;
	delete[] data;
	free(cfg);
}

void AudioCapture::update()
{
	
}

float AudioCapture::getWaveform(float i)
{
	return boost * waveform[(int)(i*waveformSize)].r;
}

float AudioCapture::getFFT(float i)
{
	i *= i;
	return (i+0.1) * 10 * fft[(int)(i*waveformSize / 2)].r;
}

float AudioCapture::getFFT(float a, float b)
{
	a *= a;
	b *= b;
	int lowBand = a*waveformSize / 2;
	int highBand = b*waveformSize / 2;
	float sum = 0;
	for (int c = lowBand; c <= highBand; c++)
		sum += fft[c].r;
	return sum;
}

float AudioCapture::getAmplitude()
{
	static float sum;
	if (amplitudeRecalc)
	{
		amplitudeRecalc = 0;
		sum = 0;
		for (int c = 0; c < waveformSize; c += waveformSize / 10)
		{
			sum += abs(waveform[c].r);
		}
		sum /= 10;
		sum *= boost;
		return sum;
	}
	return sum;
}

void AudioCapture::normalise()
{
	float amp = getAmplitude()/boost;
	boost = (int)(20 / amp);
}