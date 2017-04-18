#include "ActionView.h"
#include "UISlider.h"

ActionView::ActionView(int x, int y, int w, int h, Action* act) : UIElement(x, y, w, h), action(act)
{
	
	actionType.setString(getActionTypeString());
	actionType.setFillColor(sf::Color::White);
	actionType.setFont(*UIElement::getFont());
	actionType.setCharacterSize(12);
	
	actionAmount.setString(std::to_string(action->getAmount()));
	actionAmount.setFillColor(sf::Color::White);
	actionAmount.setFont(*UIElement::getFont());
	actionAmount.setCharacterSize(12);

	addChild(std::make_unique<UISlider>(x, y + 24, w, 8, action->getAmount(), 0, 2));//
}


ActionView::~ActionView()
{
}

void ActionView::update()
{
	auto slider = dynamic_cast<UISlider*>(children[0].get());//eww
	action->setAmount(slider->getValue());

	actionAmount.setString(std::to_string(action->getAmount()));


	draw(actionType);
	draw(actionAmount);
}

void ActionView::refresh()
{
	actionType.setPosition(x + 4, y + 4);
	actionAmount.setPosition(x + 64, y + 4);
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