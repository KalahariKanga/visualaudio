#include "ParameterListView.h"


ParameterListView::ParameterListView(int x, int y, int w, int h, std::vector<Parameter*>* params) : UIElement(x, y, w, h), parameters(params)
{
	int i = 0;
	for (auto& p : *parameters)
	{
		addChild<ParameterView>(x, y + i * parameterHeight, w, parameterHeight, p);
		++i;
	}
}


ParameterListView::~ParameterListView()
{
	
}

void ParameterListView::update()
{

}

void ParameterListView::refresh()
{
	
}

void ParameterListView::processEvent(sf::Event ev)
{
	
}