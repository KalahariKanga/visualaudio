#include "UISlider.h"
#include <sstream>

UISlider::UISlider(int x, int y, int w, int h, float value, float min, float max) : UIElement(x, y, w, h), min(min), max(max), value(value)
{
	valueText.setFillColor(sf::Color::White);
	valueText.setFont(*UIElement::getFont());
	valueText.setCharacterSize(12);
	fill.setFillColor(sf::Color::Red);
	outline.setFillColor(sf::Color(0, 0, 0, 0));
	outline.setOutlineColor(sf::Color::White);
	outline.setOutlineThickness(1);
}


UISlider::~UISlider()
{
}

void UISlider::update()
{
	std::stringstream stream;
	stream << value;
	valueText.setString(stream.str());

	sf::Vector2f sz(value/(max-min) * w, h);
	fill.setSize(sz);

	draw(outline);
	draw(fill);
	draw(valueText);
}

void UISlider::refresh()
{
	outline.setPosition(x, y);
	outline.setSize(sf::Vector2f(w, h));
	fill.setPosition(x, y);

	valueText.setPosition(x, y);
}

void UISlider::processEvent(sf::Event ev)
{
	if (ev.type == sf::Event::MouseMoved)
	{
		if (Math::pointInRect(ev.mouseMove.x, ev.mouseMove.y, x, y, x + w, y + h))
		{
			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
			{
				value = min + (max-min)*((float)(ev.mouseMove.x - x) / w);
			}
		}
	}

	if (ev.type == sf::Event::MouseButtonPressed)
	{
		if (Math::pointInRect(ev.mouseButton.x, ev.mouseButton.y, x, y, x + w, y + h))
		{
			if (ev.mouseButton.button == sf::Mouse::Button::Right)
			{
				//parameter->setValue(parameter->getDefaultValue());
			}
			if (ev.mouseButton.button == sf::Mouse::Button::Left)
			{
				value = min + (max - min)*((float)(ev.mouseButton.x - x) / w);
			}
		}
	}
}