#include "UISlider.h"
#include <sstream>

UISlider::UISlider(int x, int y, int w, int h, float value, float min, float max) : UIElement(x, y, w, h), min(min), max(max), value(value)
{
	defValue = value;
	setup();
}

UISlider::UISlider(int x, int y, int w, int h, Parameter* p) : UIElement(x,y,w,h), parameter(p)
{
	defValue = parameter->getDefaultValue();
	min = parameter->getMin();
	max = parameter->getMax();
	value = parameter->getValue();
	setup();
}


UISlider::~UISlider()
{
}

void UISlider::setup()
{
	valueText.setFillColor(UIStyle::Colour::Primary);
	valueText.setFont(*UIElement::getFont());
	valueText.setCharacterSize(UIStyle::Text::fontSize);
	fill.setFillColor(UIStyle::Colour::Secondary);
	outline.setFillColor(sf::Color(0, 0, 0, 0));
	outline.setOutlineColor(UIStyle::Colour::Primary);
	outline.setOutlineThickness(1);
}

int UISlider::valueToXPos(float v)
{
	auto perc = (v - min) / (max - min);
	return x + w*perc;
}

void UISlider::update()
{
	if (parameter)
		setValue(parameter->getValue());
	std::stringstream stream;
	stream << value;
	valueText.setString(stream.str());

	sf::Vector2f sz(valueToXPos(value) - valueToXPos(Math::clamp(0, min, max)), h);
	fill.setSize(sz);

	draw(outline);
	draw(fill);
	draw(valueText);
}

void UISlider::refresh()
{
	outline.setPosition(x, y);
	outline.setSize(sf::Vector2f(w, h));
	
	fill.setPosition(Math::clamp(valueToXPos(0), x, x + w), y);
	valueText.setPosition(x + w/2, y-4);
}

void UISlider::processEvent(sf::Event ev)
{
	if (ev.type == sf::Event::MouseMoved)
	{
		if (mouseDown)
		{
			if (ev.mouseMove.y >= y - scrubMargin && ev.mouseMove.y < y + h + scrubMargin)
				setValue(min + (max - min)*((float)(ev.mouseMove.x - x) / w));
			else
			{
				float targetValue = min + (max - min)*((float)(ev.mouseMove.x - x) / w);
				float startValue = min + (max - min)*((float)(mouseDownX - x) / w);
				float scrubScale = Math::clamp(4. / abs(y + h / 2 - ev.mouseMove.y), 0.05, 1);
				setValue(Math::lint(startValue, targetValue, scrubScale));
			}
		}
	}

	if (ev.type == sf::Event::MouseButtonPressed)
	{
		if (Math::pointInRect(ev.mouseButton.x, ev.mouseButton.y, x, y, x + w, y + h))
		{
			if (ev.mouseButton.button == sf::Mouse::Button::Right)
			{
				setValue(defValue);
			}
			if (ev.mouseButton.button == sf::Mouse::Button::Left)
			{
				mouseDown = 1;
				mouseDownX = ev.mouseButton.x;
				setValue(min + (max - min)*((float)(ev.mouseButton.x - x) / w));
			}
		}
	}
	if (ev.type == sf::Event::MouseButtonReleased)
	{
		if (ev.mouseButton.button == sf::Mouse::Button::Left)
			mouseDown = 0;
	}
}