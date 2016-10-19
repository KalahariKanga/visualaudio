#include "UIElement.h"

bool UIElement::loadedFont = 0;
sf::Font UIElement::font;

UIElement::UIElement(int _x, int _y, int _w, int _h) : x(_x), y(_y), w(_w), h(_h)
{
}


UIElement::~UIElement()
{
}

void UIElement::addChild(std::unique_ptr<UIElement> child)
{
	children.emplace_back(std::move(child));
	children.back()->parent = this;
	children.back()->texture = texture;
}

void UIElement::resize(int _w, int _h)
{
	w = _w;
	h = _h;
}

sf::Font* UIElement::getFont()
{
	if (!loadedFont)
	{
		font.loadFromFile("consola.ttf");
		loadedFont = 1;
	}
	return &font;
}