#include "Parameter.h"


Parameter::Parameter(std::string n, float v, float min, float max) : name(n), value(v), min(min), max(max)
{
	interval = (max - min) / 10;
}


Parameter::~Parameter()
{
}
