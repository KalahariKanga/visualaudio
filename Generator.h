#pragma once
#include <vector>
#include <algorithm>
#include "Parameter.h"
#include "Canvas.h"
#include "AudioCapture.h"

class Generator
{
	
	std::vector<Parameter> parameters;
	void addParameter(std::string name, float value, float min, float max);
protected:
	AudioCapture* const ac;
public:
	Generator(AudioCapture* AC);
	~Generator();
	virtual void update(Canvas& target);//draw
};

