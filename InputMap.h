#pragma once
#include <map>
#include "Action.h"
#include "InputEvent.h"

class InputMap
{
	using MapType = std::multimap < InputButton, Action > ;
	MapType map;
public:
	InputMap();
	~InputMap();

	void addAction(InputButton input, Action action);//s/action/link?

	void recieveEvent(InputEvent ev);
	std::vector<std::pair<InputButton*, Action*>> findParameterActions(Parameter* p);
	void removeLink(std::pair<InputButton, Action> link);
};

