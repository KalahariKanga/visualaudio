#pragma once
#include "PopupWindow.h"
class ParameterActionWindow :
	public PopupWindow
{
	Parameter* p;
public:
	ParameterActionWindow(Parameter* param);
	~ParameterActionWindow();

	void update();
};

