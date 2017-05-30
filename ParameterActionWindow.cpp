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
	notNew = 1;//this is sooo dumbbbbb
	draw(title);
}

void ParameterActionPanel::processEvent(sf::Event ev)
{
	//scrolling
	if (ev.type == sf::Event::MouseWheelMoved)
	{
		if (ev.mouseWheel.x > x && ev.mouseWheel.x < x + w)
		{
			if (y + getH() > texture->getSize().y && ev.mouseWheel.delta < 0)
			{
				offsetPosition(0, ev.mouseWheel.delta * 16); //magic
				needRefresh = 1;
			}
			if (ev.mouseWheel.delta > 0 && y < 0)
			{
				offsetPosition(0, ev.mouseWheel.delta * 16);
				needRefresh = 1;
			}
		}
	}
	//click away
	if (ev.type == sf::Event::MouseButtonPressed && notNew)
	{
		if (!Math::pointInRect(ev.mouseButton.x, ev.mouseButton.y, x, y, x + w, y + h))
			toQuit = 1;
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
	if (needRebuild)
	{
		rebuildChildren();
		needRebuild = 0;
	}
	title.setPosition(x + wPad, y + hPad);
	repositionChildren(0, hStep + hStep);

}

void ParameterActionPanel::addLink()
{
	inputMap->addAction(InputButton(InputButton::Device::None, -1), Action(p, Action::Type::set));
	needRebuild = 1;
}

void ParameterActionPanel::removeLink(std::pair<InputButton, Action> action)
{
	inputMap->removeLink(action);
	needRebuild = 1;
}