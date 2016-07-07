#pragma once
#include <cstdlib>
class Math
{
public:
	Math();
	~Math();
	static float clamp(float v, float min, float max)
	{
		if (v < min)
			return min;
		if (v > max)
			return max;
		return v;
	}
	static float random(float min, float max)
	{
		float r = (float)rand() / (float)RAND_MAX;
		return min + r * (max - min);
	}
};

