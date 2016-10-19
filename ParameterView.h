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
	int paddingX = 8, paddingY = 8;
	int sliderH = 16;
public:
	ParameterView(int x, int y, int w, int h, Parameter* parameter);
	~ParameterView();
	virtual void update();
	void resize(int w, int h);
};

