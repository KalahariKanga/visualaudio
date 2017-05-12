#include "UIButton.h"


UIButton::UIButton(int x, int y, int w, int h, std::function<void(void)> _onClick, std::string str) : UIElement(x, y, w, h)
{
	onClick = _onClick;
	text.setFont(*UIElement::getFont());
	text.setString(str);
	text.setPosition(x, y);
	text.setCharacterSize(UIStyle::Text::fontSize);
	text.setFillColor(UIStyle::Colour::Secondary);
	rectangle.setFillColor(sf::Color(0, 0, 0, 0));
	rectangle.setOutlineColor(UIStyle::Colour::Primary);
	rectangle.setOutlineThickness(1);
}


UIButton::~UIButton()
{
}

void UIButton::update()
{
	rectangle.setPosition(x, y);
	rectangle.setSize(sf::Vector2f(w, h));

	text.setPosition(x+1, y-6);

	draw(rectangle);
	draw(text);
}

void UIButton::processEvent(sf::Event ev)
{
	if (ev.type == sf::Event::MouseButtonPressed)
	{
		if (Math::pointInRect(ev.mouseButton.x, ev.mouseButton.y, x, y, x + w, y + h))
		{
			if (ev.mouseButton.button == sf::Mouse::Button::Left)
				onClick();
		}
	}
}

void UIButton::refresh()
{

}