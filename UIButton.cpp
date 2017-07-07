#include "UIButton.h"
#include "App.h"

using namespace UIStyle::Layout;

UIButton::UIButton(int x, int y, int w, int h, std::function<void(void)> _onClick, std::string str) : UIElement(x, y, w, h)
{
	onClick = _onClick;
	text.setString(str);
	setup();
}

UIButton::UIButton(int x, int y, int w, int h, Parameter* parameter_, std::string str) : UIElement(x, y, w, h), parameter(parameter_)
{
	text.setString(str);
	onClick = [&](){ parameter->trigger(); };
	setup();
}

UIButton::~UIButton()
{
}

void UIButton::setup()
{
	text.setFont(*UIElement::getFont());
	text.setPosition(x, y);
	text.setCharacterSize(UIStyle::Text::fontSize);
	text.setFillColor(UIStyle::Colour::Secondary);
	rectangle.setFillColor(sf::Color(0, 0, 0, 0));
	rectangle.setOutlineColor(UIStyle::Colour::Primary);
	rectangle.setOutlineThickness(1);
}

void UIButton::update()
{
	rectangle.setPosition(x, y);
	rectangle.setSize(sf::Vector2f(w, h));

	text.setPosition(x+1, y-2);

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
			if (ev.mouseButton.button == sf::Mouse::Button::Middle && parameter)
				App::parameterActionPopup(parameter);
		}
	}
	if (ev.type == sf::Event::MouseMoved)
	{
		if (Math::pointInRect(ev.mouseMove.x, ev.mouseMove.y, x, y, x + w, y + hStep - 1))
			rectangle.setOutlineColor(UIStyle::Colour::PrimaryHighlight);
		else
			rectangle.setOutlineColor(UIStyle::Colour::Primary);
	}
}

void UIButton::refresh()
{

}