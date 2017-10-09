#include "AudioCapture.h"
#include "Math.h"
#include <iostream>

static int callback(const void *inputBuffer, void *outputBuffer,
	unsigned long framesPerBuffer,
	const PaStreamCallbackTimeInfo* timeInfo,
	PaStreamCallbackFlags statusFlags,
	void *userData)
{
	auto ac = (AudioCapture*)userData;
	for (unsigned c = 0; c < ac->waveformSize*(ac->fftSize-1); c++)
	{
		ac->longWaveform[c] = ac->longWaveform[c +ac-> waveformSize];
	}
	for (unsigned c = 0; c < framesPerBuffer; c+=ac->noChannels)//TODO:ASSUMES STEREO (might be fixed)
	{
		float a = ((float*)inputBuffer)[c];
		float b = ((float*)inputBuffer)[c + 1];
		ac->waveform[c / 2].r = (a + b) / 2.0;
		ac->longWaveform[(ac->fftSize-1)*ac->waveformSize + c / 2].r = (a + b) / 2.0;
	}
	ac->amplitudeRecalc = 1;
	
	kiss_fft(ac->cfg, ac->longWaveform, ac->fft);

	return paContinue;
}

AudioCapture::AudioCapture()
{
	Pa_Initialize();
	PaStreamParameters inputParameters;
	auto n = Pa_GetDefaultInputDevice();
	auto info = Pa_GetDeviceInfo(n);
	noChannels = inputParameters.channelCount = info->maxInputChannels;
	inputParameters.device = n;
	inputParameters.sampleFormat = paFloat32;
	inputParameters.suggestedLatency = info->defaultLowInputLatency;
	inputParameters.hostApiSpecificStreamInfo = NULL;
	Pa_OpenStream(&stream, &inputParameters, NULL, 44100, bufferSize, paNoFlag, callback, (void*)this);//magic
	Pa_StartStream(stream);

	waveformSize = bufferSize / noChannels;

	waveform = new kiss_fft_cpx[waveformSize];
	fft = new kiss_fft_cpx[fftSize*waveformSize];
	longWaveform = new kiss_fft_cpx[fftSize*waveformSize];

	cfg = kiss_fft_alloc(fftSize*waveformSize, 0, NULL, NULL);

	for (int c = 0; c < waveformSize; c++)
	{
		waveform[c].i = 0;
		fft[c].r = fft[c].i = 0;
		longWaveform[c].i = 0;
	}
}


AudioCapture::~AudioCapture()
{
	Pa_StopStream(stream);
	Pa_CloseStream(stream);
	Pa_Terminate();
	delete[] waveform;
	delete[] fft;
	delete[] longWaveform;
	free(cfg);
}

void AudioCapture::update()
{

}

float AudioCapture::getWaveform(float i)
{
	i = Math::clamp(i, 0, 0.999);
	return Math::clamp(boost * waveform[(int)(i*waveformSize)].r,-2,2);
}

float AudioCapture::getFFT(float i)
{
	/*i *= i;
	return (i+0.1) * 10 * fft[(int)(i*waveformSize*fftSize / 2)].r;*/
	return fft[(int)(i*waveformSize*fftSize / 2)].r;
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
	boost = (int)(1 / (2 * amp));
}