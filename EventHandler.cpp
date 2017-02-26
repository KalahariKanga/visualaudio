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

		inputMap.recieveEvent(ev);
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

void EventHandler::setInputMaps(std::vector<InputMap> maps)
{
	inputMap.clear();
	for (int c = 0; c < maps.size(); c++)
		inputMap.merge(maps[c]);
}