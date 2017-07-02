#include "GeneratorView.h"
#include "UIComboBox.h"
using namespace UIStyle::Layout;

GeneratorView::GeneratorView(int x, int y, int w, int h, Scene* sc) : UIElement(x, y, w, h), scene(sc)
{
	generator = scene->getGenerator();
	name.setFont(*UIElement::getFont());
	name.setCharacterSize(UIStyle::Text::fontSize);
	name.setFillColor(UIStyle::Colour::Primary);

	if (generator)
		list = generator->getParameterList();
	rebuildChildren();
	
}


GeneratorView::~GeneratorView()
{
}

void GeneratorView::update()
{
	name.setPosition(x + hStep, y + hPad);
	name.setString(scene->getGenerator()->getName());
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
	/*if (ev.type == sf::Event::MouseMoved)
	{
		if (ev.mouseMove.y > y - 4 && ev.mouseMove.y < y + h + 4 && ev.mouseMove.x < w)
		{
			for (int c = 2; c < children.size(); c++)
				children[c]->setActive(true);
		}
		else
		{
			for (int c = 2; c < children.size(); c++)
				children[c]->setActive(false);
		}
	}*/
}

void GeneratorView::triggerCollapse()
{
	collapsed = !collapsed;
	children[0]->triggerCollapse();
	requestRefresh();
}

void GeneratorView::rebuildChildren()
{
	children.clear();
	addChild<UIComboBox>(x, y + 5 * hStep, w - 2 * hPad, sliderH, Generator::getGeneratorList(),
		[&](std::string str)
	{
		scene->setGenerator(Generator::construct(str));
		generator = scene->getGenerator();
		rebuildChildren();
	});

	if (generator)
		list = generator->getParameterList();
	else
		list.clear();

	addChild<ParameterListView>(x, y + hStep, w, 0, &list);
	addChild<UIButton>(hPad, y + hStep / 2, buttonSize, buttonSize, [&](){ triggerCollapse(); });
	repositionChildren();
	requestRefresh();
}