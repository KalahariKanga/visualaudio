#pragma once
#include "ParameterView.h"
class ParameterListView :
	public UIElement
{
	std::vector<Parameter*>* parameters;
	int parameterHeight = 48;
public:
	ParameterListView(int x, int y, int w, int h, std::vector<Parameter*>* params);
	~ParameterListView();
	virtual void update();
	virtual void processEvent(sf::Event ev);
	void refresh();
};

