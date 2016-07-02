#pragma once
#include "Canvas.h"
#include "AudioCapture.h"
class Generator
{
	AudioCapture* const ac;
public:
	Generator(AudioCapture* AC);
	~Generator();
	virtual void update(Canvas& target);//draw
};

