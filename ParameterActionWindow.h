#pragma once
#include "UIPanel.h"
#include "InputMap.h"
#include "LinkView.h"

class ParameterActionPanel :
	public UIElement
{
	Parameter* p;
	InputMap* inputMap;

	sf::Text title;
	void addLink();

	bool needRebuild = 0;
public:
	ParameterActionPanel(int w, int h, Parameter* param, InputMap* map, sf::RenderTexture* tex);
	~ParameterActionPanel();

	virtual void update();
	virtual void processEvent(sf::Event ev);
	virtual void refresh();

	void rebuildChildren();//:/
	void removeLink(std::pair<InputButton, Action> action);
	bool toQuit = 0;
};

