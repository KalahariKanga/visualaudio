#include "ShaderView.h"
#include "UIButton.h"
#include "ShaderListView.h"

using namespace UIStyle::Layout;

ShaderView::ShaderView(int x, int y, int w, int h, Shader* sh) : UIElement(x, y, w, h), shader(sh)
{
	name.setFont(*UIElement::getFont());
	name.setCharacterSize(UIStyle::Text::fontSize);
	name.setFillColor(UIStyle::Colour::Primary);

	//refresh();
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
	name.setPosition(x + 2 * hStep, y + 2);
	name.setString(shader->getName());
	draw(name);
}

void ShaderView::refresh()
{
	list = shader->getParameterList();
	auto bypassParam = std::find(list.begin(), list.end(), shader->getParameter("bypass"));
	list.erase(bypassParam);

	children.clear();
	addChild<ParameterListView>(x, y + hStep, w, 0, &list);
	int by = y + hPad;
	addChild<UIButton>(x + hPad, by, buttonSize, buttonSize, [&](){ triggerCollapse(); });
	addChild<UIButton>(x + w - hPad - buttonSize, by, buttonSize, buttonSize, [=](){ remove(); squashEvent(); }, "x");
	addChild<UIButton>(x + w - 2 * (hPad + buttonSize), by, buttonSize, buttonSize, shader->getParameter("bypass"), "b");
	addChild<UIButton>(x + w - 3 * (hPad + buttonSize), by, buttonSize, buttonSize, [=](){ move(-1); squashEvent(); }, "^");
	addChild<UIButton>(x + w - 4 * (hPad + buttonSize), by, buttonSize, buttonSize, [=](){ move(1); squashEvent(); }, "v");

}

void ShaderView::processEvent(sf::Event ev)
{
	
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
		p->refresh();
	}
}

void ShaderView::move(int delta)
{
	auto p = dynamic_cast<ShaderListView*>(parent);
	if (p)
	{
		p->moveShader(this, delta);
		p->refresh();//bit odd
	}
}