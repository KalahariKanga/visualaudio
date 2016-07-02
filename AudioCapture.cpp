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
}


AudioCapture::~AudioCapture()
{
	delete[] waveform;
}

void AudioCapture::update()
{
	while (BASS_ChannelGetData(recordHandle, 0, BASS_DATA_AVAILABLE) < bufferSize)
	{ }
	BASS_ChannelGetData(recordHandle, waveform, 4 * 2 * bufferSize);
}