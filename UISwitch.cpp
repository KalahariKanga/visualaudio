#include "UISwitch.h"
#include "App.h"

using namespace UIStyle::Colour;
using namespace UIStyle::Layout;

UISwitch::UISwitch(int x, int y, int w, int h, Parameter* p) : UIElement(x,y,w,h), parameter(p)
{
	state = parameter->getValue();
	setup();
}

UISwitch::UISwitch(int x, int y, int w, int h, bool state) : UIElement(x, y, w, h), state(state)
{
	setup();
}

UISwitch::~UISwitch()
{
}

void UISwitch::setup()
{
	outline.setFillColor(sf::Color(0, 0, 0, 0));
	outline.setOutlineColor(Primary);
	outline.setOutlineThickness(1);

	fill.setOutlineColor(sf::Color(0, 0, 0, 0));
}

void UISwitch::update()
{
	if (parameter)
		state = parameter->getValue();
	if (state)
	{
		fill.setFillColor(SecondaryHighlight);
		fill.setPosition(x + 2 * w / 3, y);
	}
	else
	{
		fill.setFillColor(Secondary);
		fill.setPosition(x, y);
	}
	draw(outline);
	draw(fill);
}

void UISwitch::refresh()
{
	outline.setPosition(x, y);
	outline.setSize(sf::Vector2f(w, h));

	fill.setSize(sf::Vector2f(w/3, h));
}

void UISwitch::processEvent(sf::Event ev)
{
	if (ev.type == sf::Event::MouseButtonPressed)
	{
		if (Math::pointInRect(ev.mouseButton.x, ev.mouseButton.y, x, y, x + w, y + h))
		{
			if (ev.mouseButton.button == sf::Mouse::Button::Left)
			{
				setState(!getState());//O.o
			}
			if (ev.mouseButton.button == sf::Mouse::Button::Middle)
			{
				App::parameterActionPopup(parameter);
			}
		}
	}
	if (ev.type == sf::Event::MouseMoved)
	{
		if (Math::pointInRect(ev.mouseMove.x, ev.mouseMove.y, x, y, x + w, y + hStep - 1))
			outline.setOutlineColor(UIStyle::Colour::PrimaryHighlight);
		else
			outline.setOutlineColor(UIStyle::Colour::Primary);
	}
}