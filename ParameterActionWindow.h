#pragma once
#include "PopupWindow.h"
#include "InputMap.h"
#include "LinkView.h"

class ParameterActionWindow :
	public PopupWindow
{
	Parameter* p;
	std::vector<InputMap*> inputMaps;

	sf::Text tmp;
public:
	ParameterActionWindow(int w, int h, Parameter* param, std::vector<InputMap*> maps);
	~ParameterActionWindow();

	void update();
	void refresh();
	InputEvent detectNextEvent();//probably need an rtmidi ptr :/

	void updateLink(std::pair<InputButton, Action> from, std::pair<InputButton, Action> to);

};

