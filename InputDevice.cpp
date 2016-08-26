#include "InputDevice.h"


InputDevice::InputDevice()
{
}


InputDevice::~InputDevice()
{
}

void InputDevice::update()
{
	while (!eventList.empty())
	{
		int code = eventList.front();
		eventList.pop_front();
		auto action = inputMap.find(code);
		if (action != inputMap.end())
			(action->second).execute();
	}
}

void InputDevice::addAction(int code, Action act)
{
	inputMap[code] = act;
}