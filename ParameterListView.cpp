#include "ParameterListView.h"


ParameterListView::ParameterListView(int x, int y, int w, int h) : UIElement(x,y,w,h)
{
	texture = new sf::RenderTexture();
	texture->create(w, h);
}


ParameterListView::~ParameterListView()
{
	delete texture;
}

void ParameterListView::update()
{
	texture->clear(sf::Color(0,0,0,128));
	texture->display();
	for (auto& pv : children)
	{
		pv->update();
	}
	texture->display();
}

void ParameterListView::refresh(std::vector<Parameter*> list)
{
	children.clear();
	int i = 0;
	for (auto& p : list)
	{
		addChild(std::make_unique<ParameterView>(x, y + i*parameterHeight, w, parameterHeight, p));
		++i;
	}
}