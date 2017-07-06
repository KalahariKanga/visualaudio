#include "GeneratorView.h"
#include "UIComboBox.h"
using namespace UIStyle::Layout;

GeneratorView::GeneratorView(int x, int y, int w, int h, Scene* sc) : UIElement(x, y, w, h), scene(sc)
{
	if (scene)
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
	name.setPosition(x + 2 * hStep, y + 2);
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
	children[2]->triggerCollapse();
	requestRefresh();
}

void GeneratorView::rebuildChildren()
{
	children.clear();
	addChild<UIButton>(x + hPad, y + hPad, buttonSize, buttonSize, [&](){ triggerCollapse(); });
	addChild<UIComboBox>(x + 2 * hPad + buttonSize, y + hPad, w - 3 * hPad - buttonSize, sliderH, Generator::getGeneratorList(),
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
	
	//repositionChildren();
	requestRefresh();
}