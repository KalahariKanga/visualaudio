#pragma once
#include "PopupWindow.h"
#include "InputMap.h"
#include "LinkView.h"

class ParameterActionWindow :
	public PopupWindow
{
	Parameter* p;
	InputMap* inputMap;

	sf::Text title;
public:
	ParameterActionWindow(int w, int h, Parameter* param, InputMap* map);
	~ParameterActionWindow();

	virtual void update();
	virtual void processEvent(sf::Event ev);
	virtual void refresh();
	InputEvent detectNextEvent();//probably need an rtmidi ptr :/

	void rebuildChildren();//:/
};

