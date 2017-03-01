#include "ParameterActionWindow.h"


ParameterActionWindow::ParameterActionWindow(int w, int h, Parameter* param, InputMap map) : PopupWindow(w, h), p(param), inputMap(map)
{
	window.create(sf::VideoMode(128, 386), "ParameterActionWindow");
	texture = new sf::RenderTexture();
	texture->create(128, 386);
	//find all input/action pairs using p
	auto actions = inputMap.findParameterActions(p);
	for (auto &a : actions)
	{
		addChild((std::make_unique<ActionView>(x, y, w, 16, a.first, a.second)));
	}
}


ParameterActionWindow::~ParameterActionWindow()
{
	delete texture;
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

	



	PopupWindow::update();
	texture->clear();
}

void ParameterActionWindow::refresh()
{
	for (int i = 1; i < children.size(); i++)
	{
		int newY = children[i - 1]->getY() + children[i - 1]->getH();
		children[i]->setPosition(children[i]->getX(), newY);
	}
}