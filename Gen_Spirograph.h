#pragma once
#include "Generator.h"
class Gen_Spirograph :
	public Generator
{
	float time = 0;
public:
	Gen_Spirograph(AudioCapture* ac);
	~Gen_Spirograph();
	virtual void update(Canvas& target, float deltaTime);
};

