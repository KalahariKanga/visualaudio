#pragma once
#include "Generator.h"
class Gen_Spectrum :
	public Generator
{
public:
	Gen_Spectrum(AudioCapture* ac);
	~Gen_Spectrum();
	
	virtual void update(Canvas& target);
};

