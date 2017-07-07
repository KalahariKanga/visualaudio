#pragma once
#include "UIElement.h"
class UIButton :
	public UIElement
{
	std::function<void(void)> onClick;
	sf::RectangleShape rectangle;
	sf::Text text;
	Parameter* parameter = nullptr;
	void setup();
public:
	UIButton(int x, int y, int w, int h, std::function<void(void)> _onClick, std::string text = "");
	UIButton(int x, int y, int w, int h, Parameter* parameter_, std::string text = "");
	~UIButton();
	virtual void update();
	virtual void processEvent(sf::Event ev);
	virtual void refresh();
};

