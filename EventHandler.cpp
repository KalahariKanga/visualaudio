#include "EventHandler.h"


EventHandler::EventHandler()
{
}


EventHandler::~EventHandler()
{
}

void EventHandler::update()
{
	while (!eventList.empty())
	{
		InputEvent ev = eventList.front();
		eventList.pop_front();

		inputMap->recieveEvent(ev);
	}
}

void EventHandler::addEvent(InputEvent ev)
{
	eventList.push_back(ev);
}

void EventHandler::addEvent(InputButton::Device device, int button, float data)
{
	eventList.emplace_back(InputButton(device, button), data);
}

void EventHandler::setInputMap(InputMap* map)
{
	inputMap = map;
}

InputButton EventHandler::nextButton()
{
	int sz = eventList.size();
	for (auto it = eventList.begin(); it != eventList.end(); it++)
	{
		if (it->button.device == InputButton::Device::Audio)
			continue;
		if (it->button.device == InputButton::Device::GamepadAxis)
			if (abs(it->data) < 0.1)
				continue;//deadzone
		return it->button;
	}
	return InputButton(InputButton::Device::None, 0);
}