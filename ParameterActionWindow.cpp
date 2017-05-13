#include "ParameterActionWindow.h"

using namespace UIStyle::Layout;

ParameterActionPanel::ParameterActionPanel(int w, int h, Parameter* param, InputMap* map, sf::RenderTexture* tex) : UIElement(192,0,w,h), p(param), inputMap(map)
{
	texture = tex;
	title.setString(param->getName());
	title.setFillColor(UIStyle::Colour::Primary);
	title.setFont(*UIElement::getFont());
	title.setCharacterSize(UIStyle::Text::fontSize);

	addChild<UIButton>(x + hPad, y + hStep, buttonSize, buttonSize, [&](){ this->addLink(); }, "+");

	//find all input/action pairs using p
	auto actions = inputMap->findParameterActions(p);
	for (auto &a : actions)
	{
		addChild<LinkView>(x, y + hPad + hStep, w, 16, a.first, a.second);
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
	addChild<UIButton>(x + hPad, y + hStep, buttonSize, buttonSize, [&](){ this->addLink(); }, "+");
	auto actions = inputMap->findParameterActions(p);
	for (auto &a : actions)
	{
		addChild<LinkView>(x, y + hPad + hStep, w, 16, a.first, a.second);
	}

}

void ParameterActionPanel::refresh()
{
	title.setPosition(x + wPad, y + hPad);
	for (int i = 1; i < children.size(); i++)
	{
		int newY = children[i - 1]->getY() + children[i - 1]->getH();
		children[i]->setPosition(children[i]->getX(), newY);
	}//DRY
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