#pragma once
#include "Generator.h"
class Gen_Waveform :
	public Generator
{
public:
	Gen_Waveform(AudioCapture* ac);
	~Gen_Waveform();
	virtual void update(Canvas& target, float deltaTime);	
	virtual std::string getName() override { return "Waveform"; }

};

