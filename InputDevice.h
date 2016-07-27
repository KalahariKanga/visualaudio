#pragma once
#include "Action.h"
#include <list>

class InputDevice
{
protected:
	std::map<int, Action> inputMap;
	std::list<int> eventList;
public:
	InputDevice();
	~InputDevice();
	void update();
	void addAction(int code, Action act);
};

