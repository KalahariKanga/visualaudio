#include "ParameterView.h"
#include "UIButton.h"
#include "UISlider.h"
#include "UISwitch.h"
#include <sstream>

using namespace UIStyle::Layout;

ParameterView::ParameterView(int _x, int _y, int _w, int _h, Parameter* _parameter) : UIElement(_x, _y, _w, _h)
{
	
	parameter = _parameter;
	
	name.setString(parameter->getName());
	name.setFillColor(UIStyle::Colour::Primary);
	name.setFont(*UIElement::getFont());
	name.setCharacterSize(UIStyle::Text::fontSize);

	if (parameter->type == Parameter::Type::Continuous || parameter->type == Parameter::Type::Discrete)
	{
		int sx = x + wPad;
		int sy = y + h - hPad - sliderH;
		addChild<UISlider>(sx, sy, w - 2 * hPad, sliderH, parameter);
	}

	if (parameter->type == Parameter::Type::Switch)
	{
		int sx = x + wPad;
		int sy = y + h - hPad - sliderH;
		addChild<UISwitch>(sx, sy, w - 2 * hPad, sliderH, parameter);
	}

	if (parameter->type == Parameter::Type::Trigger)
	{
		int sx = x + wPad;
		int sy = y + h - hPad - sliderH;
		addChild<UIButton>(sx, sy, w - 2 * hPad, sliderH, [&](){ parameter->trigger(); });
	}
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
	name.setPosition(x + wPad, y + h - hPad - sliderH - 2);
}

void ParameterView::processEvent(sf::Event ev)
{
	
}