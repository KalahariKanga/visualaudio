#include "ParameterListView.h"


ParameterListView::ParameterListView(int x, int y, int w, int h, std::vector<Parameter*>* params) : UIElement(x, y, w, h), parameters(params)
{

}


ParameterListView::~ParameterListView()
{
	
}

void ParameterListView::update()
{

}

void ParameterListView::refresh()
{
	children.clear();
	int i = 0;
	for (auto& p : *parameters)
	{
		addChild(std::make_unique<ParameterView>(x, y + i*parameterHeight, w, parameterHeight, p));
		++i;
	}
}

void ParameterListView::processEvent(sf::Event ev)
{
	
}