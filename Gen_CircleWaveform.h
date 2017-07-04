#pragma once
#include "Generator.h"
class Gen_CircleWaveform :
	public Generator
{
public:
	Gen_CircleWaveform(AudioCapture* ac);
	~Gen_CircleWaveform();
	virtual void update(Canvas& target, float deltaTime);
};

