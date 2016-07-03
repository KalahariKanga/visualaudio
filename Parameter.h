#pragma once
#include <string>
#include "Math.h"

class Parameter
{
	float value, min, max, interval;
	std::string name;
public:
	Parameter(std::string n, float v, float min, float max);
	~Parameter();
	std::string getName(){ return name; }
	float getValue(){ return value; }
	void setValue(float v)
	{
		value = Math::clamp(v, min, max);
	}
	void increment(){ value = Math::clamp(value + interval, min, max); }
	void decrement(){ value = Math::clamp(value - interval, min, max); }
};

