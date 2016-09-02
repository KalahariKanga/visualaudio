#pragma once
#include "Parameter.h"

class Action
{
	Parameter* target;
	float amount;
public:
	enum class Type
	{
		set, shift, trigger
	} type;
	
	Action();
	Action(Parameter* target, Action::Type type, float amount = 0);
	~Action();

	void execute(float eventValue = 1);
	
};

