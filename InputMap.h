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

	void addAction(InputButton input, Action action);
	void clear();
	void merge(InputMap other);

	void recieveEvent(InputEvent ev);
};

