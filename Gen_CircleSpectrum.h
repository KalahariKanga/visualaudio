#pragma once
#include "Generator.h"
class Gen_CircleSpectrum :
	public Generator
{
	int bands = 8; //24
	std::vector<float> bandAmount;
public:
	Gen_CircleSpectrum(AudioCapture* AC);
	~Gen_CircleSpectrum();
	virtual void update(Canvas& target, float deltaTime);
};

