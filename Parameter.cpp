#include "Parameter.h"


Parameter::Parameter(float v, float min, float max) : value(v), min(min), max(max)
{
	interval = (max - min) / 10;
}


Parameter::~Parameter()
{
}
