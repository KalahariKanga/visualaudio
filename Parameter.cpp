#include "Parameter.h"

std::vector<Parameter*> Parameter::untriggerParameters;

Parameter::Parameter(std::string name, float v, float min, float max, Type type) : name(name), value(v), min(min), max(max), type(type)
{
	def = v;
}


Parameter::~Parameter()
{
}
