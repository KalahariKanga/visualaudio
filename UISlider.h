#pragma once
#include "UIElement.h"
class UISlider :
	public UIElement
{
	sf::RectangleShape fill, outline;
	sf::Text valueText;
	float min, max, value, defValue;
	bool mouseDown = 0;
	int mouseDownX = 0;
	Parameter* parameter = nullptr;

	int scrubMargin = 8;

	void setup();
	int valueToXPos(float v);
public:
	UISlider(int x, int y, int w, int h, float value, float min, float max);
	UISlider(int x, int y, int w, int h, Parameter* p);
	~UISlider();

	virtual void update();
	virtual void processEvent(sf::Event ev);
	virtual void refresh();

	float getValue() { return value; }
	void setValue(float v)
	{
		v = Math::clamp(v, min, max);
		value = v;
		if (parameter)
			parameter->setValue(v); 
	}
};

