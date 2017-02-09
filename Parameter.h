#pragma once
#include <string>
#include <map>
#include "Math.h"
#include "SFML/graphics.hpp"

class Parameter
{
	std::string name;
	float value, min, max, def;
public:
	Parameter(std::string name, float v, float min, float max);
	~Parameter();
	std::string getName()
	{
		return name;
	}
	float getDefaultValue()
	{
		return def;
	}
	float getValue()
	{
		return value;
	}
	void setValue(float v)
	{
		value = Math::clamp(v, min, max);
	}
	float getNormalisedValue()
	{
		return (value - min) / (max - min);
	}
	void setNormalisedValue(float v)
	{
		v = Math::clamp(v, 0, 1);
		value = min + v*(max - min);
	}
};

