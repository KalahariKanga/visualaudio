#pragma once
#include <list>

#include "InputMap.h"


class EventHandler
{
	
	InputMap* inputMap;
	std::list<InputEvent> eventList;
public:
	EventHandler();
	~EventHandler();
	void update();
	void addEvent(InputEvent ev);
	void addEvent(InputButton::Device device, int button, float data = 1);
	InputMap* getInputMap(){ return inputMap; }
	void setInputMap(InputMap* map);
};

