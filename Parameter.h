#pragma once
#include <string>
#include <map>
#include "Math.h"
#include "SFML/graphics.hpp"

class Parameter
{
	std::string name;
	float value, min, max, def;
	bool lock = 0;
public:
	Parameter(std::string name, float v, float min, float max);
	~Parameter();
	std::string getName() const
	{
		return name;
	}
	float getDefaultValue(){ return def; }
	float getValue(){ return value;	}
	float getMin() const { return min; }
	float getMax() const { return max; }
	void setValue(float v)
	{
		if (!lock)
			value = Math::clamp(v, min, max);
	}
	float getNormalisedValue()
	{
		return (value - min) / (max - min);
	}
	void setNormalisedValue(float v)
	{
		if (!lock)
		{
			v = Math::clamp(v, 0, 1);
			value = min + v*(max - min);
		}
	}
	void setLock(bool l)
	{
		lock = l;
	}
};

