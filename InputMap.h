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
	void clear();
	void merge(InputMap other);
	void updateLink(std::pair<InputButton, Action> from, std::pair<InputButton, Action> to);

	void recieveEvent(InputEvent ev);
	InputMap::MapType findParameterActions(Parameter* p);
};

