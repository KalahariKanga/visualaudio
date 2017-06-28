#pragma once
#include "UIElement.h"
class UISubPanel :
	public UIElement
{
public:
	UISubPanel(int w, int h);
	~UISubPanel();
	bool toQuit = 0;
	bool notNew = 0;
};

