#include "InputReciever.h"
#include <iostream>

InputReciever::InputReciever()
{
}


InputReciever::~InputReciever()
{
}

void InputReciever::addParameter(std::string name, float value, float min, float max, float interval)
{
	parameters.insert(std::map<std::string, Parameter>::value_type(name, Parameter(name, value, min, max, interval)));
}

Parameter* InputReciever::getParameter(std::string name)
{
	try
	{
		return &parameters.at(name);
	}
	catch (std::out_of_range)
	{
		std::cout << "Error: Unknown Parameter Name: " << name << "\n";
		return nullptr;
	}
}

std::vector<Parameter*> InputReciever::getParameterList()
{
	std::vector<Parameter*> list;
	for (auto &p : parameters)
	{
		list.push_back(&(p.second));
	}
	return list;
}