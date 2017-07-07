#include "UIElement.h"

bool UIElement::loadedFont = 0;
sf::Font UIElement::font;

UIElement::UIElement(int _x, int _y, int _w, int _h) : x(_x), y(_y), w(_w), h(_h)
{

}


UIElement::~UIElement()
{
}

void UIElement::removeChild(UIElement* elem)
{
	toRemove.push_back(elem);
}

void UIElement::repositionChildren(int offsetX, int offsetY)
{
	//only use for pure containers?
	if (!children.empty())
		children[0]->setPosition(x + offsetX, y + offsetY);
	for (int i = 1; i < children.size(); i++)
	{
		int newY = children[i - 1]->getY() + children[i - 1]->getH();
		children[i]->setPosition(children[i]->getX(), newY);
	}
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

void UIElement::processEvent(sf::Event ev)
{
	//assert
}

void UIElement::refresh()
{

}

void UIElement::distributeEvent(sf::Event ev)
{
	//initially was rangefor, but iterator got 
	//invalidated after adding new shaders on the fly
	processEvent(ev);
	for (int c = 0; c < children.size(); c++)
	{
		if (c >= children.size()) break;
		if (children.at(c)->isActive())
			children.at(c)->distributeEvent(ev);
	}
}

void UIElement::doUpdate()
{
	//remove deferred deletions
	for (auto &toRem : toRemove)
	{
		auto it = std::find_if(children.begin(), children.end(),
			[toRem](std::unique_ptr<UIElement>& elem){ return (elem.get() == (UIElement*)toRem);});
		if (it != children.end())
			children.erase(it);
	}
	toRemove.clear();

	
	for (auto& c : children)
	{
		if (c->isActive())
			c->doUpdate();
	}
	update();

	if (needRefresh)
	{
		doRefresh();
		needRefresh = 0;
	}
}

void UIElement::doRefresh()
{
	refresh();
	for (auto& c : children)
	{
		c->doRefresh();
	}
}

sf::Texture UIElement::getTexture()
{
	if (!parent)
		return texture->getTexture();//could be null
	return parent->getTexture();
}

void UIElement::draw(const sf::Drawable& src)
{
	if (!parent)
		texture->draw(src);
	else
		parent->draw(src);
}

void UIElement::setPosition(int _x, int _y)
{
	offsetPosition(_x - x, _y - y);
}

void UIElement::offsetPosition(int dx, int dy)
{
	for (auto& ch : children)
	{
		ch->offsetPosition(dx, dy);
	}
	x += dx;
	y += dy;
}

int UIElement::getH()
{
	if (children.empty())
		return h;
	int min = y, max = y + h;
	for (auto& ch : children)
	{
		if (ch->isActive() && !ch->collapsed)
		{
			int bottom = ch->getY() + ch->getH();
			if (bottom > max)
				max = bottom;
		}
	}
	
	return max - min;;
}

int UIElement::getW()
{
	if (children.empty())
		return w;
	int min = x;
	int max = std::numeric_limits<int>().min();
	for (auto& ch : children)
	{
		int right = ch->getX() + ch->getW();
		if (right > max)
			max = right;
	}
	w = max - min;
	return w;
}

void UIElement::triggerCollapse()
{
	collapsed = !collapsed;
	for (auto &c : children)
		c->setActive(!collapsed);
	requestRefresh();
}