#include "ParameterView.h"
#include <sstream>

ParameterView::ParameterView(int _x, int _y, int _w, int _h, Parameter* _parameter) : UIElement(_x, _y, _w, _h)
{
	parameter = _parameter;
	
	resize(w, h);
	
	name.setString(parameter->getName());
	name.setFillColor(sf::Color::White);
	value.setFillColor(sf::Color::White);
	name.setFont(*UIElement::getFont());
	value.setFont(*UIElement::getFont());
	name.setCharacterSize(12);
	value.setCharacterSize(12);
	sliderFill.setFillColor(sf::Color::Red);
	sliderOutline.setFillColor(sf::Color::Blue);
}


ParameterView::~ParameterView()
{
}

void ParameterView::update()
{
	std::stringstream stream;
	stream << parameter->getValue();
	value.setString(stream.str());

	sf::Vector2f sz(parameter->getNormalisedValue() * (w - 2 * paddingY), sliderH);
	sliderFill.setSize(sz);
	
	texture->draw(sliderOutline);
	texture->draw(sliderFill);
	texture->draw(name);
	texture->draw(value);
}

void ParameterView::resize(int _w, int _h)
{
	UIElement::resize(_w, _h);
	sliderOutline.setPosition(paddingX, h - paddingY - sliderH);
	sliderOutline.setSize(sf::Vector2f(w - 2 * paddingY, sliderH));
	sliderFill.setPosition(x + paddingX, y + h - paddingY - sliderH);

	name.setPosition(x + paddingX, y + paddingY);
	
	value.setPosition(x + _w / 2, y + paddingY);
}

void ParameterView::processEvent(sf::Event ev)
{
	if (ev.type == sf::Event::MouseMoved)
	{
		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
			if (ev.mouseMove.x > x && ev.mouseMove.x < x+w)
				if (ev.mouseMove.y > y && ev.mouseMove.y < y + h)
				{
					parameter->setNormalisedValue((float)(ev.mouseMove.x - x) / w);
				}
	}
}