#pragma once
#include <list>
#include "InputEvent.h"
#include "Action.h"
class EventHandler
{
	using InputMap = std::multimap < InputButton, Action >;
	InputMap inputMap;
	std::list<InputEvent> eventList;
public:
	EventHandler();
	~EventHandler();
	void update();
	void addEvent(InputEvent ev);
	void addEvent(InputButton::Device device, int button, float data = 1);
	void addAction(InputButton input, Action action);
};

