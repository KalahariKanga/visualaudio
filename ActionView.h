#pragma once
#include <string>
#include "Action.h"
#include "InputEvent.h"
#include "UIElement.h"


class ActionView :
	public UIElement
{
	InputButton button;
	Action action;

	sf::Text deviceName, buttonName;
	sf::Text actionType;

public:
	ActionView(int x, int y, int w, int h, InputButton button, Action action);
	~ActionView();

	virtual void update();
	/*virtual void processEvent(sf::Event ev);*/
	virtual void refresh();
};

