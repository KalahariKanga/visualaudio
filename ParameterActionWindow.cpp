#include "ParameterActionWindow.h"


ParameterActionWindow::ParameterActionWindow(Parameter* param) : p(param)
{
	window.create(sf::VideoMode(128, 386), "ParameterActionWindow");
}


ParameterActionWindow::~ParameterActionWindow()
{
	
}

void ParameterActionWindow::update()
{
	sf::Event ev;
	while (window.pollEvent(ev))
	{
		if (ev.type == sf::Event::Closed)
		{
			quit = 1;
		}
	}
	window.display();
}