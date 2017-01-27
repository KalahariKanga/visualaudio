#include "ShaderView.h"
#include "UIButton.h"
#include "ShaderListView.h"

ShaderView::ShaderView(int x, int y, int w, int h, Shader* sh) : UIElement(x, y, w, h), shader(sh)
{
	name.setFont(*UIElement::getFont());
	name.setCharacterSize(12);
	name.setFillColor(sf::Color::White);

	list = shader->getParameterList();
	addChild(std::make_unique<ParameterListView>(x, y + 16, w, 0, &list));
	addChild(std::make_unique<UIButton>(4, y + 8, 8, 8, [&](){ triggerCollapse(); }));
	addChild(std::make_unique<UIButton>(x + w - 10, y + 8, 8, 8, [=](){ remove(); }, "x"));
	addChild(std::make_unique<UIButton>(x + w - 18, y + 8, 8, 8, [=](){ shader->setActive(!shader->isActive()); }, "b"));
	addChild(std::make_unique<UIButton>(x + w - 26, y + 4, 8, 8, [=](){ move(-1); }));
	addChild(std::make_unique<UIButton>(x + w - 26, y + 12, 8, 8, [=](){ move(1); }));
}


ShaderView::~ShaderView()
{
}

void ShaderView::update()
{
	if (shader->isActive())
		name.setFillColor(sf::Color::White);
	else
		name.setFillColor(sf::Color(92, 92, 92));
	name.setPosition(x + 16, y + 4);
	name.setString(shader->getName());
	texture->draw(name);
}

void ShaderView::refresh()
{
	list = shader->getParameterList();
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