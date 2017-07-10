#pragma once
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <sstream>

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
	static bool pointInRect(float px, float py, float x1, float y1, float x2, float y2)
	{
		if (x1 > x2) std::swap(x1, x2);
		if (y1 > y2) std::swap(y1, y2);
		if (px < x1) return 0;
		if (px > x2) return 0;
		if (py < y1) return 0;
		if (py > y2) return 0;
		return 1;
	}
	static std::string toSigFigString(float x, int sigFigs = 3)
	{
		std::stringstream stream;
		stream.precision(sigFigs);
		stream << x;
		return stream.str();
	}
};

