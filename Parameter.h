#pragma once
#include <string>

float clamp(float v, float min, float max)
{
	if (v < min)
		return min;
	if (v > max)
		return max;
	return v;
}

class Parameter
{
	float value, min, max;
	std::string name;
public:
	Parameter(std::string n, float v, float min, float max);
	~Parameter();
	std::string getName(){ return name; }
	float getValue(){ return value; }
	void setValue(float v)
	{
		value = clamp(v, min, max);
	}
};

