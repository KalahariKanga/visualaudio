#include "GeneratorView.h"


GeneratorView::GeneratorView(int x, int y, int w, int h, Generator* gen) : UIElement(x, y, w, h), generator(gen)
{
	name.setFont(*UIElement::getFont());
	name.setCharacterSize(12);
	name.setFillColor(sf::Color::White);

	list = generator->getParameterList();
	addChild(std::make_unique<ParameterListView>(x, y + 16, w, 200, &list));
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
	if (generator)
		list = generator->getParameterList();
}

void GeneratorView::processEvent(sf::Event ev)
{
	if (ev.type == sf::Event::MouseButtonReleased)
	{
		if (Math::pointInRect(ev.mouseButton.x, ev.mouseButton.y, x, y, x + w, y + 16))
		{
			collapsed = !collapsed;
			for (auto &c : children)
				c->setActive(!collapsed);
			requestRefresh();
		}
	}
}