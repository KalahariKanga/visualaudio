#include "GeneratorView.h"


GeneratorView::GeneratorView(int x, int y, int w, int h, Generator* gen) : UIElement(x, y, w, h), generator(gen)
{
	name.setFont(*UIElement::getFont());
	name.setCharacterSize(12);
	name.setFillColor(sf::Color::White);
}


GeneratorView::~GeneratorView()
{
}

void GeneratorView::update()
{
	name.setPosition(x + 16, y + 8);
	//name.setString(generator->getName());
	name.setString("Generator");
	texture->draw(name);
}

void GeneratorView::refresh()
{
	children.clear();
	if (!generator) return;
	list = generator->getParameterList();
	addChild(std::make_unique<ParameterListView>(x, y + 16, w, 200, &list));
}