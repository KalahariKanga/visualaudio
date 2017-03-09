#pragma once
#include "Parameter.h"

class Action
{
	Parameter* target;
	float amount;
public:
	enum class Type
	{
		set, setNormalised, shift, trigger, axis
	} type;
	
	Action();
	Action(Parameter* target, Action::Type type, float amount = 0);
	~Action();

	void execute(float eventValue = 1);
	bool usesParameter(Parameter* p){ return (p == target); }
	const Parameter getTarget(){ return *target; }
	float getAmount(){ return amount; }
};

