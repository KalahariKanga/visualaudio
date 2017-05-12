#include "GeneratorView.h"


GeneratorView::GeneratorView(int x, int y, int w, int h, Generator* gen) : UIElement(x, y, w, h), generator(gen)
{
	name.setFont(*UIElement::getFont());
	name.setCharacterSize(UIStyle::Text::fontSize);
	name.setFillColor(UIStyle::Colour::Primary);

	list = generator->getParameterList();
	addChild<ParameterListView>(x, y + 16, w, 0, &list);
	addChild<UIButton>(4, y + 8, 8, 8, [&](){ triggerCollapse(); });
}


GeneratorView::~GeneratorView()
{
}

void GeneratorView::update()
{
	name.setPosition(x + 16, y + 8);
	//name.setString(generator->getName());
	name.setString("Generator");
	draw(name);
}

void GeneratorView::refresh()
{
	if (generator)
		list = generator->getParameterList();
}

void GeneratorView::processEvent(sf::Event ev)
{
	
}

void GeneratorView::triggerCollapse()
{
	collapsed = !collapsed;
	children[0]->triggerCollapse();
	requestRefresh();
}