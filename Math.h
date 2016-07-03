#pragma once
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
};

