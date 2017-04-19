#include "ActionView.h"
#include "UISlider.h"

ActionView::ActionView(int x, int y, int w, int h, Action* act) : UIElement(x, y, w, h), action(act)
{
	
	actionType.setString(getActionTypeString());
	actionType.setFillColor(sf::Color::White);
	actionType.setFont(*UIElement::getFont());
	actionType.setCharacterSize(12);

	float min = action->getTarget()->getMin();
	float max = action->getTarget()->getMax();
	addChild(std::make_unique<UISlider>(x, y + 24, w, 8, action->getAmount(), min, max));
}


ActionView::~ActionView()
{
}

void ActionView::update()
{
	auto slider = dynamic_cast<UISlider*>(children[0].get());//eww
	action->setAmount(slider->getValue());

	actionType.setString(getActionTypeString());

	draw(actionType);
}

void ActionView::processEvent(sf::Event ev)
{
	if (ev.type == sf::Event::MouseButtonPressed)
	{
		if (Math::pointInRect(ev.mouseButton.x, ev.mouseButton.y, x, y, x + w, y + h / 2))
		{
			cycleActionType();
		}
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

void ActionView::cycleActionType()
{
	switch (action->type)
	{
	case Action::Type::set:
		action->type = Action::Type::shift;
		break;
	case Action::Type::shift:
		action->type = Action::Type::trigger;
		break;
	case Action::Type::trigger:
		action->type = Action::Type::axis;
		break;
	case Action::Type::axis:
		action->type = Action::Type::set;
		break;
	default:
		action->type = Action::Type::set;
	}
}