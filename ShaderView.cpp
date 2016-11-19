#include "ShaderView.h"


ShaderView::ShaderView(int x, int y, int w, int h, Shader* sh) : UIElement(x, y, w, h), shader(sh)
{ 
	name.setFont(*UIElement::getFont());
	name.setCharacterSize(12);
	name.setFillColor(sf::Color::White);

	list = shader->getParameterList();
	addChild(std::make_unique<ParameterListView>(x, y + 16, w, 200, &list));
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
	if (ev.type == sf::Event::MouseButtonReleased)
	{
		if (Math::pointInRect(ev.mouseButton.x, ev.mouseButton.y, x, y, x + w, y + 16))
		{
			collapsed = !collapsed;
			for (auto &c : children)
				c->setActive(!collapsed);
			requestRefresh();
			//requestRefresh();
		}
	}
}