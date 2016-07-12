#pragma once
#include <vector>
#include <algorithm>
#include "Parameter.h"
#include "Canvas.h"
#include "AudioCapture.h"

class Generator
{
	
	std::map<std::string, Parameter> parameters;
protected:
	AudioCapture* const ac;
	void addParameter(std::string name, float value, float min, float max);
public:
	Generator(AudioCapture* AC);
	~Generator();
	virtual void update(Canvas& target);//draw
	Parameter& getParameter(std::string name);

};

