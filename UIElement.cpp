#include "UIElement.h"

bool UIElement::loadedFont = 0;
sf::Font UIElement::font;
sf::RenderTexture* UIElement::texture;

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
	//children.back()->texture = texture;
	children.back()->doRefresh();
}

void UIElement::removeChild(UIElement* elem)
{
	toRemove.push_back(elem);
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
		std::cout << c << "/" << children.size() << "\n";
		if (c >= children.size()) break;
		if (children[c]->isActive())
			children[c]->distributeEvent(ev);
	}
	
}

void UIElement::doUpdate()
{
	update();
	for (auto& c : children)
	{
		if (c->isActive())
			c->doUpdate();
	}

	//remove deferred deletions
	for (auto &toRem : toRemove)
	{
		auto it = std::find_if(children.begin(), children.end(),
			[toRem](std::unique_ptr<UIElement>& elem){ return (elem.get() == (UIElement*)toRem); });
		children.erase(it);
	}
	toRemove.clear();
}

void UIElement::doRefresh()
{
	refresh();
	for (auto& c : children)
	{
		c->doRefresh();
	}
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
		if (ch->isActive())
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