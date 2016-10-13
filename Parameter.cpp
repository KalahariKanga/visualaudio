#include "Parameter.h"


Parameter::Parameter(std::string name, float v, float min, float max, float interval) : name(name), value(v), min(min), max(max), interval(interval)
{
	if (interval == -1)
		interval = (max - min) / 10;
}


Parameter::~Parameter()
{
}
