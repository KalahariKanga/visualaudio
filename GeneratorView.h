#pragma once
#include "ParameterListView.h"
#include "UIElement.h"
#include "Generator.h"
class GeneratorView :
	public UIElement
{
	Generator* generator = nullptr;
	std::vector<Parameter*> list;
	sf::Text name;
public:
	GeneratorView(int x, int y, int w, int h, Generator* generator);
	~GeneratorView();
	virtual void update();
	virtual void refresh();
};

