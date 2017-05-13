#include "ParameterView.h"
#include "UIButton.h"
#include "UISlider.h"
#include <sstream>

using namespace UIStyle::Layout;

std::function<void(Parameter*)> ParameterView::popupCall;

ParameterView::ParameterView(int _x, int _y, int _w, int _h, Parameter* _parameter) : UIElement(_x, _y, _w, _h)
{
	
	parameter = _parameter;
	
	name.setString(parameter->getName());
	name.setFillColor(UIStyle::Colour::Primary);
	name.setFont(*UIElement::getFont());
	name.setCharacterSize(UIStyle::Text::fontSize);

	int sx = x + wPad;
	int sy = y + h - hPad - sliderH;
	addChild<UISlider>(sx, sy, w - 4 * hPad, sliderH, parameter);

	int bx = w - 3 * wPad;
	int by = y + h - hPad - sliderH;
	addChild<UIButton>(bx, by, 2 * wPad, sliderH, std::bind(popupCall, parameter));
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
	name.setPosition(x + wPad, y + hPad);
}