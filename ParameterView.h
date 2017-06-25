#pragma once
#include "UIElement.h"
#include "SFML/Graphics.hpp"
class ParameterView :
	public UIElement
{
	Parameter* parameter;
	
	sf::Text name;
	
public:
	ParameterView(int x, int y, int w, int h, Parameter* parameter);
	~ParameterView();
	virtual void update();
	virtual void refresh();
	virtual void processEvent(sf::Event ev);

	//this is bad
	static std::function<void(Parameter*)> popupCall;
};

