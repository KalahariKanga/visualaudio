#include "ParameterActionWindow.h"


ParameterActionPanel::ParameterActionPanel(int w, int h, Parameter* param, InputMap* map, sf::RenderTexture* tex) : UIElement(192,0,w,h), p(param), inputMap(map)
{
	texture = tex;
	title.setString(param->getName());
	title.setFillColor(sf::Color::White);
	title.setFont(*UIElement::getFont());
	title.setCharacterSize(12);

	addChild<UIButton>(x + 8, y + 16, 8, 8, [&](){ this->addLink(); }, "+");

	//find all input/action pairs using p
	auto actions = inputMap->findParameterActions(p);
	for (auto &a : actions)
	{
		addChild<LinkView>(x, y + 20, w, 16, a.first, a.second);
	}
	
}


ParameterActionPanel::~ParameterActionPanel()
{

}

void ParameterActionPanel::update()
{
	draw(title);
}

void ParameterActionPanel::processEvent(sf::Event ev)
{
	if (ev.type == sf::Event::KeyPressed)
	{
		if (ev.key.code == sf::Keyboard::Space)
		{
			addLink();
		}
	}
}

void ParameterActionPanel::rebuildChildren()
{
	children.clear();
	addChild<UIButton>(x + 8, y + 16, 8, 8, [&](){ this->addLink(); }, "+");
	auto actions = inputMap->findParameterActions(p);
	for (auto &a : actions)
	{
		addChild<LinkView>(x, y + 20, w, 16, a.first, a.second);
	}
}

void ParameterActionPanel::refresh()
{
	title.setPosition(x + 4, y + 4);
	for (int i = 1; i < children.size(); i++)
	{
		int newY = children[i - 1]->getY() + children[i - 1]->getH();
		children[i]->setPosition(children[i]->getX(), newY);
	}
}

void ParameterActionPanel::addLink()
{
	inputMap->addAction(InputButton(InputButton::Device::None, -1), Action(p, Action::Type::set));
	rebuildChildren();
}

void ParameterActionPanel::removeLink(std::pair<InputButton, Action> action)
{
	inputMap->removeLink(action);
	rebuildChildren();
}