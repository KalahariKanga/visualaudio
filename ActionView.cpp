#include "ActionView.h"
#include "LinkView.h"
#include "UISlider.h"

using namespace UIStyle::Layout;

ActionView::ActionView(int x, int y, int w, int h, InputButton* but, Action* act) : UIElement(x, y, w, h), action(act), button(but)
{
	lastDevice = button->device;
	actionType.setString(getActionTypeString());
	actionType.setFillColor(UIStyle::Colour::Primary);
	actionType.setFont(*UIElement::getFont());
	actionType.setCharacterSize(UIStyle::Text::fontSize);

	float min = action->getTarget()->getMin();
	float max = action->getTarget()->getMax();
	addChild<UISlider>(x + wPad, y + hStep, w - 2 * hPad, sliderH, action->getAmount(), min, max);//actually created in update bounds
	updateBounds();
}


ActionView::~ActionView()
{
}

void ActionView::update()
{
	auto slider = dynamic_cast<UISlider*>(children[0].get());//eww
	action->setAmount(slider->getValue());

	//if the inputbutton has changed, we might need to change the type
	//this is all kinda hacky, but will need a rewrite when params get typed
	if (button->device != lastDevice)
	{
		action->type = Action::Type::axis;
		nextLegalActionType();
	}
	lastDevice = button->device;

	actionType.setString(getActionTypeString());

	draw(actionType);
}

void ActionView::processEvent(sf::Event ev)
{
	if (ev.type == sf::Event::MouseButtonPressed)
	{
		if (Math::pointInRect(ev.mouseButton.x, ev.mouseButton.y, x, y, x + w, y + hStep - 1))
		{
			nextLegalActionType();
		}
	}

	if (ev.type == sf::Event::MouseMoved)
	{
		if (Math::pointInRect(ev.mouseMove.x, ev.mouseMove.y, x, y, x + w, y + hStep - 1))
			actionType.setFillColor(UIStyle::Colour::PrimaryHighlight);
		else
			actionType.setFillColor(UIStyle::Colour::Primary);
	}
}

void ActionView::refresh()
{
	actionType.setPosition(x + wPad, y);
}

std::string ActionView::getActionTypeString()
{
	std::string ret;
	switch (action->type)
	{
	case Action::Type::set:
		return "Set to";
	case Action::Type::shift:
		return "Shift by";
	case Action::Type::trigger:
		return "Trigger";
	case Action::Type::axis:
		return "Control by";
	default:
		return "";
	}
}

void ActionView::nextLegalActionType()
{
	switch (button->device)
	{
	case InputButton::Device::Audio:
	case InputButton::Device::GamepadAxis:
	case InputButton::Device::MIDICV:
		switch (action->getTarget()->type)
		{
		case Parameter::Type::Continuous:
		case Parameter::Type::Discrete:
		case Parameter::Type::Switch://i think?
			cycle({ Action::Type::axis });
			break;
		case Parameter::Type::Trigger:
			cycle({ Action::Type::trigger });
			break;
		}
		break;
	case InputButton::Device::GamepadButton:
	case InputButton::Device::Keyboard:
	case InputButton::Device::MIDINote:
		switch (action->getTarget()->type)
		{
		case Parameter::Type::Continuous:
		case Parameter::Type::Discrete:
			cycle({ Action::Type::set, Action::Type::shift });
			break;
		case Parameter::Type::Switch:
			cycle({ Action::Type::set, Action::Type::trigger });
			break;
		case Parameter::Type::Trigger:
			cycle({ Action::Type::trigger });
			break;
		}
		break;
	}
	updateBounds();
}

void ActionView::updateBounds()
{
	float min, max;
	switch (action->type)
	{
	case Action::Type::set:
		min = action->getTarget()->getMin();
		max = action->getTarget()->getMax();
		break;
	case Action::Type::shift:
		min = -(action->getTarget()->getMax());
		max = action->getTarget()->getMax();
		break;
	case Action::Type::trigger:
		min = 0;
		max = 1;//for now
		break;
	case Action::Type::axis:
		min = -1;
		max = 1;
		break;
	}

	children.erase(children.begin());
	addChild<UISlider>(x + wPad, y + hStep, w - 2 * hPad, sliderH, action->getAmount(), min, max);
}

void ActionView::cycle(std::vector<Action::Type> list)
{
	for (int c = 0; c < list.size(); c++)
	{
		if (list[c] == action->type)
		{
			action->type = list[(c + 1) % list.size()];
			return;
		}
	}
	action->type = list[0];
}