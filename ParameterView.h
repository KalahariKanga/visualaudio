#pragma once
#include "UIElement.h"
#include "SFML/Graphics.hpp"
class ParameterView :
	public UIElement
{
	Parameter* parameter;
	sf::RectangleShape sliderFill;
	sf::RectangleShape sliderOutline;
	
	sf::Text name;
	sf::Text value;
	int paddingX = 4, paddingY = 4;
	int sliderH = 14;
public:
	ParameterView(int x, int y, int w, int h, Parameter* parameter);
	~ParameterView();
	virtual void update();
	virtual void processEvent(sf::Event ev);
	virtual void refresh();
};

