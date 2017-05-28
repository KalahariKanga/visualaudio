#pragma once
#include "UIElement.h"
#include "InputEvent.h"
#include "Action.h"

class ActionView :
	public UIElement
{
	Action* action;
	InputButton* button;
	InputButton::Device lastDevice;
	sf::Text actionType;

	std::string getActionTypeString();
	Action::Type nextActionType();
	void nextLegalActionType();
	void updateBounds();
	void cycle(std::vector<Action::Type> list);
public:
	ActionView(int x, int y, int w, int h, InputButton* but, Action* act);
	~ActionView();

	virtual void update();
	virtual void refresh();
	virtual void processEvent(sf::Event ev);
};

