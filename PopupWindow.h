#pragma once
#include "UIPanel.h"

class PopupWindow : public UIElement
{
protected:
	sf::RenderWindow window;
	bool quit = 0;
public:
	PopupWindow(int w, int h);
	~PopupWindow();

	bool toQuit(){ return quit; }

	void update() override;
};

