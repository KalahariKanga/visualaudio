#include "AudioCapture.h"
#include <iostream>

AudioCapture::AudioCapture()
{
	BASS_Init(-1, 44100, 0, 0, 0);
	/*
	BASS_SetConfig(BASS_CONFIG_BUFFER, 128);
	BASS_SetConfig(BASS_CONFIG_UPDATEPERIOD, 10);
	*/
	BASS_RecordInit(-1);
	recordHandle = BASS_RecordStart(44100, 2, BASS_SAMPLE_FLOAT, 0, 0);
	waveform = new float[bufferSize];
	FFT = new float[bufferSize/2];
}


AudioCapture::~AudioCapture()
{
	delete[] waveform;
	delete[] FFT;
}

void AudioCapture::update()
{
	amplitudeRecalc = 1;
	while (BASS_ChannelGetData(recordHandle, 0, BASS_DATA_AVAILABLE) < bufferSize)
	{ }
	BASS_ChannelGetData(recordHandle, waveform, sizeof(float) * 2 * bufferSize);
	BASS_ChannelGetData(recordHandle, FFT, BASS_DATA_FFT1024);
	BASS_ChannelGetData(recordHandle, 0, 0xFFFFFFF);//clear buffer
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