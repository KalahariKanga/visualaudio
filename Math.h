#pragma once
#include <cmath>
#include <cstdlib>
#include <iostream>

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
	static float lint(float a, float b, float x)
	{
		return a + x*(b - a);
	}
	static float point_direction(float x1, float y1, float x2, float y2)
	{
		return atan2(y2 - y1 , x2 - x1);
	}
	static float point_distance(float x1, float y1, float x2, float y2)
	{
		return sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1));
	}
};

