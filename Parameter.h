#pragma once
#include <string>
#include "Math.h"

class Parameter
{
	float value, min, max, interval;
	bool changed = 1;
public:
	Parameter(float v, float min, float max);
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
	bool hasChanged(){ return changed; }
};

