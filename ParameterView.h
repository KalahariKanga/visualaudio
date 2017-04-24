#pragma once
#include "UIElement.h"
#include "SFML/Graphics.hpp"
class ParameterView :
	public UIElement
{
	Parameter* parameter;
	
	sf::Text name;

	int paddingX = 4, paddingY = 4;
	int sliderH = 14;
public:
	ParameterView(int x, int y, int w, int h, Parameter* parameter);
	~ParameterView();
	virtual void update();
	virtual void refresh();

	//this is bad
	static std::function<void(Parameter*)> popupCall;
};

