#include "UISlider.h"
#include <sstream>

UISlider::UISlider(int x, int y, int w, int h, float value, float min, float max) : UIElement(x, y, w, h), min(min), max(max), value(value)
{
	defValue = value;
	valueText.setFillColor(sf::Color::White);
	valueText.setFont(*UIElement::getFont());
	valueText.setCharacterSize(12);
	fill.setFillColor(sf::Color::Red);
	outline.setFillColor(sf::Color(0, 0, 0, 0));
	outline.setOutlineColor(sf::Color::White);
	outline.setOutlineThickness(1);
}

UISlider::UISlider(int x, int y, int w, int h, Parameter* p) : UIElement(x,y,w,h), parameter(p)
{
	defValue = parameter->getDefaultValue();
	min = parameter->getMin();
	max = parameter->getMax();
	value = parameter->getValue();
	valueText.setFillColor(sf::Color::White);
	valueText.setFont(*UIElement::getFont());
	valueText.setCharacterSize(12);
	fill.setFillColor(sf::Color::Red);
	outline.setFillColor(sf::Color(0, 0, 0, 0));
	outline.setOutlineColor(sf::Color::White);
	outline.setOutlineThickness(1);
}//DRY


UISlider::~UISlider()
{
}

void UISlider::update()
{
	if (parameter)
		setValue(parameter->getValue());
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
	fill.setPosition(x + w*(-min)/(max-min), y);

	valueText.setPosition(x + w/2, y-4);
}

void UISlider::processEvent(sf::Event ev)
{
	if (ev.type == sf::Event::MouseMoved)
	{
		if (mouseDown)
		{
			if (ev.mouseMove.y >= y && ev.mouseMove.y < y+h)
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