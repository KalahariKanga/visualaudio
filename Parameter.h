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
	enum class Type
	{
		Continuous, Discrete, Switch, Trigger
	}type;
	
	Parameter(std::string name, float v, float min, float max, Type type = Type::Continuous);
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
		{
			if (type == Type::Discrete)
				value = (int)round(Math::clamp(v, min, max));
			else
				value = Math::clamp(v, min, max);
		}
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
			setValue(min + v*(max - min));
		}
	}
	void setLock(bool l)
	{
		lock = l;
	}
};

