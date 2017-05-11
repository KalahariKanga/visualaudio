#pragma once
#include <string>
#include "Action.h"
#include "InputEvent.h"
#include "UIElement.h"


class LinkView :
	public UIElement
{
	InputButton* button;
	Action* action;

	sf::Text deviceName;
	sf::Text actionType;

	bool waitingForEvent = 0;
	InputButton sfEventToInputButton(sf::Event ev);

	void remove();
public:
	LinkView(int x, int y, int w, int h, InputButton* button, Action* action);
	~LinkView();

	virtual void update();
	virtual void processEvent(sf::Event ev);
	virtual void refresh();

	//this is bad too
	static std::function<InputButton(void)> nextButton;
};

