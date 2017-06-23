#pragma once
#include "Generator.h"
class Gen_Julia :
	public Generator
{

public:
	Gen_Julia(AudioCapture* ac);
	~Gen_Julia();

	virtual void update(Canvas& target, float deltaTime);
	virtual std::string getName() override { return "Julia"; }

};

