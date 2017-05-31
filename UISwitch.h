#pragma once
#include "UIElement.h"
class UISwitch :
	public UIElement
{
	bool state;
	Parameter* parameter = nullptr;
	sf::RectangleShape outline, fill;

	void setup();

public:
	UISwitch(int x, int y, int w, int h, Parameter* p);
	UISwitch(int x, int y, int w, int h, bool state);
	~UISwitch();

	virtual void update() override;
	virtual void processEvent(sf::Event ev) override;
	virtual void refresh() override;

	bool getState(){ return state; }
	void setState(bool s)
	{
		state = s;
		if (parameter)
			parameter->setValue(s);
	}
};

