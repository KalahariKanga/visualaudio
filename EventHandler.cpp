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

		auto range = inputMap.equal_range(ev.button);
		for_each(range.first, range.second,
			[ev](InputMap::value_type& x)
		{
			auto action = x.second;
			action.execute(ev.data);
		}
		);

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

void EventHandler::addAction(InputButton input, Action action)
{
	inputMap.insert(InputMap::value_type(input, action));
}