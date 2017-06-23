#include "GeneratorView.h"

using namespace UIStyle::Layout;

GeneratorView::GeneratorView(int x, int y, int w, int h, Generator* gen) : UIElement(x, y, w, h), generator(gen)
{
	name.setFont(*UIElement::getFont());
	name.setCharacterSize(UIStyle::Text::fontSize);
	name.setFillColor(UIStyle::Colour::Primary);

	if (generator)
		list = generator->getParameterList();
	addChild<ParameterListView>(x, y + hStep, w, 0, &list);
	addChild<UIButton>(hPad, y + hStep / 2, buttonSize, buttonSize, [&](){ triggerCollapse(); });
}


GeneratorView::~GeneratorView()
{
}

void GeneratorView::update()
{
	name.setPosition(x + hStep, y + hPad);
	name.setString("Generator");
	draw(name);
}

void GeneratorView::refresh()
{
	if (generator)
		list = generator->getParameterList();
	else
		list.clear();
}

void GeneratorView::processEvent(sf::Event ev)
{
	if (ev.type == sf::Event::MouseMoved)
	{
		if (ev.mouseMove.y > y - 4 && ev.mouseMove.y < y + h + 4 && ev.mouseMove.x < w)
		{
			for (int c = 1; c < children.size(); c++)
				children[c]->setActive(true);
		}
		else
		{
			for (int c = 1; c < children.size(); c++)
				children[c]->setActive(false);
		}
	}
}

void GeneratorView::triggerCollapse()
{
	collapsed = !collapsed;
	children[0]->triggerCollapse();
	requestRefresh();
}