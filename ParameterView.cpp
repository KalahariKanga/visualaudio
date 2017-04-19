#include "ParameterView.h"
#include "UIButton.h"
#include <sstream>

std::function<void(Parameter*)> ParameterView::popupCall;

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
	sliderOutline.setFillColor(sf::Color(0,0,0,0));
	sliderOutline.setOutlineColor(sf::Color::White);
	sliderOutline.setOutlineThickness(1);

	addChild(std::make_unique<UIButton>(w - 8, y, 8, 8, std::bind(popupCall,parameter)));
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
	
	draw(sliderOutline);
	draw(sliderFill);
	draw(name);
	draw(value);
}

void ParameterView::refresh()
{
	sliderOutline.setPosition(x + paddingX, y + h - paddingY - sliderH);
	sliderOutline.setSize(sf::Vector2f(w - 2 * paddingY, sliderH));
	sliderFill.setPosition(x + paddingX, y + h - paddingY - sliderH);

	name.setPosition(x + paddingX, y + paddingY);
	
	value.setPosition(x + w / 2, y + paddingY);
}

void ParameterView::processEvent(sf::Event ev)
{
	if (ev.type == sf::Event::MouseMoved)
	{
		if (Math::pointInRect(ev.mouseMove.x, ev.mouseMove.y, x, y, x+w, y+h))
		{
			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
			{
				parameter->setNormalisedValue((float)(ev.mouseMove.x - x) / w);
			}
		}
	}

	if (ev.type == sf::Event::MouseButtonPressed)
	{
		if (Math::pointInRect(ev.mouseButton.x, ev.mouseButton.y, x, y, x + w, y + h))
		{
			if (ev.mouseButton.button == sf::Mouse::Button::Right)
			{
				parameter->setValue(parameter->getDefaultValue());
			}
			if (ev.mouseButton.button == sf::Mouse::Button::Left)
			{
				parameter->setNormalisedValue((float)(ev.mouseButton.x - x) / w);
			}
		}
	}
}