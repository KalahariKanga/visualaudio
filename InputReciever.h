#pragma once
#include "Parameter.h"

class InputReciever
{
protected:
	std::map<std::string, Parameter> parameters;
	void addParameter(std::string name, float value, float min, float max);

public:
	InputReciever();
	~InputReciever();

	virtual Parameter* getParameter(std::string name);
	virtual std::vector<Parameter*> getParameterList();
};

