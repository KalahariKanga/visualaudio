#include "Parameter.h"


Parameter::Parameter(float v, float min, float max, float interval) : value(v), min(min), max(max), interval(interval)
{
	if (interval == -1)
		interval = (max - min) / 10;
}


Parameter::~Parameter()
{
}
