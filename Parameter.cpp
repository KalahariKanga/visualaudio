#include "Parameter.h"


Parameter::Parameter(std::string name, float v, float min, float max) : name(name), value(v), min(min), max(max)
{
}


Parameter::~Parameter()
{
}
