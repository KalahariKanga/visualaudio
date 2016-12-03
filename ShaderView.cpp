#include "ShaderView.h"
#include "UIButton.h"

ShaderView::ShaderView(int x, int y, int w, int h, Shader* sh) : UIElement(x, y, w, h), shader(sh)
{ 
	name.setFont(*UIElement::getFont());
	name.setCharacterSize(12);
	name.setFillColor(sf::Color::White);

	list = shader->getParameterList();
	addChild(std::make_unique<ParameterListView>(x, y + 16, w, 0, &list));
	addChild(std::make_unique<UIButton>(4, y + 8, 8, 8, [&](){ triggerCollapse(); }));
}


ShaderView::~ShaderView()
{
}

void ShaderView::update()
{
	name.setPosition(x + 16, y + 8);
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