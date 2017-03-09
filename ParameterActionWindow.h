#pragma once
#include "PopupWindow.h"
#include "InputMap.h"
#include "LinkView.h"

class ParameterActionWindow :
	public PopupWindow
{
	Parameter* p;
	InputMap inputMap;//use a ptr one day plz

	sf::Text tmp;
public:
	ParameterActionWindow(int w, int h, Parameter* param, InputMap map);
	~ParameterActionWindow();

	void update();
	void refresh();
};

