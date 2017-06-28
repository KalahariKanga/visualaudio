#include "UIComboBox.h"

using namespace UIStyle::Layout;
using namespace UIStyle::Colour;

std::function<void(UIComboBox*)> UIComboBox::popupCall;

UIComboBox::UIComboBox(int x, int y, int w, int h, std::vector<std::string> list_, std::function<void(std::string)> cb) : UIElement(x, y, w, h), items(list_), callback(cb)
{
	outline.setFillColor(sf::Color(0, 0, 0, 0));
	outline.setOutlineColor(Primary);
	outline.setOutlineThickness(1);

	text.setFillColor(Primary);
	text.setFont(*UIElement::getFont());
	text.setCharacterSize(UIStyle::Text::fontSize);

	item = "";
}


UIComboBox::~UIComboBox()
{
}

void UIComboBox::update()
{
	text.setString(item);
	text.setPosition(x + hPad, y);
	draw(outline);
	draw(text);
}

void UIComboBox::processEvent(sf::Event ev)
{
	if (ev.type == sf::Event::MouseButtonPressed)
	{
		if (ev.mouseButton.button == sf::Mouse::Button::Left)
		{
			if (Math::pointInRect(ev.mouseButton.x, ev.mouseButton.y, x, y, x + w, y + h))
			{
				popupCall(this);
			}
		}
	}
}

void UIComboBox::refresh()
{
	outline.setPosition(x + hPad, y);
	outline.setSize(sf::Vector2f(w, h));
	text.setString(item);
	text.setPosition(x + hPad, y);
}

const std::vector<std::string>& UIComboBox::getItems()
{
	return items;
}

void UIComboBox::setItem(std::string str)
{
	auto it = std::find(items.begin(), items.end(), str);
	if (it != items.end())
	{
		item = *it;
		if (callback)
			callback(str);
	}
}

std::string UIComboBox::getItem()
{
	return item;
}