#pragma once
#include "Parameter.h"

class Action
{
	Parameter* target;
	float amount, amount2;
public:
	enum class Type
	{
		set, setNormalised, shift, trigger, axis
	} type;
	
	Action();
	Action(Parameter* target, Action::Type type, float amount = 0, float amount2 = 1);
	~Action();

	void execute(float eventValue = 1);
	bool usesParameter(Parameter* p){ return (p == target); }
	const Parameter* getTarget(){ return target; }
	float getAmount(){ return amount; }
	float getAmount2(){ return amount2; }
	void setAmount(float a){ amount = a; }//friend with actionview?
	void setAmount2(float a){ amount2 = a; }
};

