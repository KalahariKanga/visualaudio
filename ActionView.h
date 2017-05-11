#pragma once
#include "UIElement.h"
#include "Action.h"

class ActionView :
	public UIElement
{
	Action* action;
	sf::Text actionType;

	std::string getActionTypeString();
	void cycleActionType();
	void updateBounds();
public:
	ActionView(int x, int y, int w, int h, Action* act);
	~ActionView();

	virtual void update();
	virtual void refresh();
	virtual void processEvent(sf::Event ev);
};

