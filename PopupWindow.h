#pragma once
#include "UIPanel.h"

class PopupWindow
{
protected:
	sf::RenderWindow window;
	bool quit = 0;
public:
	PopupWindow();
	~PopupWindow();

	bool toQuit(){ return quit; }

	virtual void update();
};

