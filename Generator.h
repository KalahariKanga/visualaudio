#pragma once
#include <vector>
#include "Parameter.h"
#include "Canvas.h"
#include "AudioCapture.h"

class Generator
{
	AudioCapture* const ac;
	std::vector<Parameter> parameters;
	void addParameter(std::string name, float value, float min, float max);

public:
	Generator(AudioCapture* AC);
	~Generator();
	virtual void update(Canvas& target);//draw
};

