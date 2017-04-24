#include "ParameterView.h"
#include "UIButton.h"
#include "UISlider.h"
#include <sstream>

std::function<void(Parameter*)> ParameterView::popupCall;

ParameterView::ParameterView(int _x, int _y, int _w, int _h, Parameter* _parameter) : UIElement(_x, _y, _w, _h)
{
	parameter = _parameter;
	
	resize(w, h);
	
	name.setString(parameter->getName());
	name.setFillColor(sf::Color::White);
	name.setFont(*UIElement::getFont());
	name.setCharacterSize(12);


	addChild(std::make_unique<UISlider>(x + paddingX, y + h - paddingY - sliderH, w - 2 * paddingY, sliderH, parameter));
	addChild(std::make_unique<UIButton>(w - 8, y, 8, 8, std::bind(popupCall,parameter)));
}


ParameterView::~ParameterView()
{
}

void ParameterView::update()
{
	std::stringstream stream;
	stream << parameter->getValue();
	
	draw(name);
}

void ParameterView::refresh()
{
	name.setPosition(x + paddingX, y + paddingY);
}