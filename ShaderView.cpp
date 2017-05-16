#include "ShaderView.h"
#include "UIButton.h"
#include "ShaderListView.h"

using namespace UIStyle::Layout;

ShaderView::ShaderView(int x, int y, int w, int h, Shader* sh) : UIElement(x, y, w, h), shader(sh)
{
	name.setFont(*UIElement::getFont());
	name.setCharacterSize(UIStyle::Text::fontSize);
	name.setFillColor(UIStyle::Colour::Primary);

	list = shader->getParameterList();

	addChild<ParameterListView>(x, y + hStep, w, 0, &list);
	int by = y + hStep / 2;
	addChild<UIButton>(hPad,							by,	buttonSize, buttonSize, [&](){ triggerCollapse(); });
	addChild<UIButton>(x + w - hPad - buttonSize,		by,	buttonSize, buttonSize, [=](){ remove(); }, "x");
	addChild<UIButton>(x + w - 2 * (hPad + buttonSize),	by,	buttonSize, buttonSize, [=](){ shader->setActive(!shader->isActive()); }, "b");
	addChild<UIButton>(x + w - 3 * (hPad + buttonSize), by, buttonSize, buttonSize, [=](){ move(-1); }, "^");
	addChild<UIButton>(x + w - 4 * (hPad + buttonSize), by, buttonSize, buttonSize, [=](){ move(1); }, "v");

	for (int c = 1; c < children.size(); c++)
		children[c]->setActive(false);//hide on init
}


ShaderView::~ShaderView()
{
}

void ShaderView::update()
{
	if (shader->isActive())
		name.setFillColor(UIStyle::Colour::Primary);
	else
		name.setFillColor(UIStyle::Colour::PrimaryLowlight);
	name.setPosition(x + hStep, y + hPad);
	name.setString(shader->getName());
	draw(name);
}

void ShaderView::refresh()
{
	list = shader->getParameterList();
}

void ShaderView::processEvent(sf::Event ev)
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

void ShaderView::triggerCollapse()
{
	collapsed = !collapsed;
	children[0]->triggerCollapse();
	requestRefresh();
}

void ShaderView::remove()
{
	auto p = dynamic_cast<ShaderListView*>(parent);
	if (p)
	{
		p->removeShader(this);
	}
}

void ShaderView::move(int delta)
{
	auto p = dynamic_cast<ShaderListView*>(parent);
	if (p)
	{
		p->moveShader(this, delta);
	}
}