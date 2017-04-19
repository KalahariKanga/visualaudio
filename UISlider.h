#pragma once
#include "UIElement.h"
class UISlider :
	public UIElement
{
	sf::RectangleShape fill, outline;
	sf::Text valueText;
	float min, max, value;
public:
	UISlider(int x, int y, int w, int h, float value, float min, float max);
	~UISlider();

	virtual void update();
	virtual void processEvent(sf::Event ev);
	virtual void refresh();

	float getValue() { return value; }
	void setValue(float v){ value = v; }
};

