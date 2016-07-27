#pragma once
#include "InputDevice.h"
class KeyboardInput :
	public InputDevice
{
public:
	KeyboardInput();
	~KeyboardInput();
	void addEvent(int e){ eventList.push_back(e); }
};

