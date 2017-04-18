#pragma once
#include "UIElement.h"
#include "Action.h"

class ActionView :
	public UIElement
{
	Action* action;
	sf::Text actionType;

	std::string getActionTypeString();
public:
	ActionView(int x, int y, int w, int h, Action* act);
	~ActionView();

	virtual void update();
	virtual void refresh();
};

