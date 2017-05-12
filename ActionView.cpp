#include "ActionView.h"
#include "LinkView.h"
#include "UISlider.h"

ActionView::ActionView(int x, int y, int w, int h, InputButton* but, Action* act) : UIElement(x, y, w, h), action(act), button(but)
{
	lastDevice = button->device;
	actionType.setString(getActionTypeString());
	actionType.setFillColor(UIStyle::Colour::Primary);
	actionType.setFont(*UIElement::getFont());
	actionType.setCharacterSize(UIStyle::Text::fontSize);

	float min = action->getTarget()->getMin();
	float max = action->getTarget()->getMax();
	addChild<UISlider>(x + 4, y + 24, w-16, 8, action->getAmount(), min, max);//actually created in update bounds
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
		if (Math::pointInRect(ev.mouseButton.x, ev.mouseButton.y, x, y, x + w, y + h / 3))
		{
			nextLegalActionType();
		}
	}

	if (ev.type == sf::Event::MouseMoved)
	{
		if (Math::pointInRect(ev.mouseMove.x, ev.mouseMove.y, x, y, x + w, y + h / 3))
			actionType.setFillColor(UIStyle::Colour::PrimaryHighlight);
		else
			actionType.setFillColor(UIStyle::Colour::Primary);
	}
}

void ActionView::refresh()
{
	actionType.setPosition(x + 4, y + 4);
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

Action::Type ActionView::nextActionType()
{
	switch (action->type)
	{
	case Action::Type::set:
		return Action::Type::shift;
		break;
	case Action::Type::shift:
		return Action::Type::trigger;
		break;
	case Action::Type::trigger:
		return Action::Type::axis;
		break;
	case Action::Type::axis:
		return Action::Type::set;
		break;
	}
}

void ActionView::nextLegalActionType()
{
	while (true)
	{
		action->type = nextActionType();
		switch (action->type)
		{
		case Action::Type::set:
		case Action::Type::shift:
		case Action::Type::trigger:
			if (button->device == InputButton::Device::Audio)
				continue;
			if (button->device == InputButton::Device::GamepadAxis)
				continue;
			if (button->device == InputButton::Device::MIDICV)
				continue;
			return;
		case Action::Type::axis:
			if (button->device == InputButton::Device::GamepadButton)
				continue;
			if (button->device == InputButton::Device::Keyboard)
				continue;
			if (button->device == InputButton::Device::MIDINote)
				continue;
			return;
		}
	}
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
	addChild<UISlider>(x + 4, y + 24, w-8, 8, 0, min, max);
}