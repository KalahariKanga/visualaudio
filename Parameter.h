#pragma once
#include <string>
#include <map>
#include "Math.h"
#include "SFML/graphics.hpp"

class Parameter
{
	float value, min, max, interval;
	bool changed = 1;
public:
	Parameter(float v, float min, float max, float interval = -1);
	~Parameter();
	float getValue()
	{
		changed = 0;
		return value;
	}
	void setValue(float v)
	{
		changed = 1; //check if it's actually different?
		value = Math::clamp(v, min, max);
	}
	void setNormalisedValue(float v)
	{
		v = Math::clamp(v, 0, 1);
		value = min + v*(max - min);
	}
	void increment()
	{
		value = Math::clamp(value + interval, min, max);
		changed = 1;
	}
	void decrement()
	{
		value = Math::clamp(value - interval, min, max);
		changed = 1;
	}
	bool hasChanged()
	{
		bool v = changed;
		changed = 0;
		return v;
	}
};

