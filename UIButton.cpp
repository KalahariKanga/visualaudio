#include "UIButton.h"


UIButton::UIButton(int x, int y, int w, int h, std::function<void(void)> _onClick) : UIElement(x, y, w, h)
{
	onClick = _onClick;
	rectangle.setFillColor(sf::Color(64, 64, 64));
	rectangle.setOutlineColor(sf::Color::White);
	rectangle.setOutlineThickness(1);
}


UIButton::~UIButton()
{
}

void UIButton::update()
{
	rectangle.setPosition(x, y);
	rectangle.setSize(sf::Vector2f(w, h));

	texture->draw(rectangle);
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