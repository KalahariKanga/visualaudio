#pragma once
#include "ParameterView.h"
class ParameterListView :
	public UIElement
{
	std::vector<std::unique_ptr<ParameterView>> parameters;
	int parameterHeight = 48;
public:
	ParameterListView(int x, int y, int w, int h);
	~ParameterListView();
	virtual void update();
	virtual void processEvent(sf::Event ev);
	void refresh(std::vector<Parameter*> list);
};

