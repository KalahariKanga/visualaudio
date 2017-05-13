#include "LinkView.h"
#include "ActionView.h"
#include "ParameterActionWindow.h"

using namespace UIStyle::Layout;

std::function<InputButton(void)> LinkView::nextButton;

LinkView::LinkView(int x, int y, int w, int h, InputButton* button, Action* action) : UIElement(x, y, w, h), button(button), action(action)
{
	deviceName.setString(button->getDeviceName() + " " + std::to_string((int)button->button));
	deviceName.setFillColor(UIStyle::Colour::Primary);
	deviceName.setFont(*UIElement::getFont());
	deviceName.setCharacterSize(UIStyle::Text::fontSize);

	addChild<ActionView>(x, y + hStep, w, 48, button, action);
	addChild<UIButton>(x + w - buttonSize - hPad, y, buttonSize, buttonSize, [&](){remove(); }, "x");
}


LinkView::~LinkView()
{
}

void LinkView::update()
{
	if (waitingForEvent)
	{
		auto newButton = nextButton();
		if (newButton.device != InputButton::Device::None)
		{
			waitingForEvent = 0;
			deviceName.setString(newButton.getDeviceName() + " " + std::to_string((int)newButton.button));
			*button = newButton;
			return;
		}
	}
	
	draw(deviceName);
}

void LinkView::processEvent(sf::Event ev)
{
	
	if (ev.type == sf::Event::MouseButtonPressed)
	{
		//cancel on either click
		if (waitingForEvent)
		{
			deviceName.setString(button->getDeviceName() + " " + std::to_string((int)button->button));
			waitingForEvent = 0;
		}
		//get input on right click
		if (Math::pointInRect(ev.mouseButton.x, ev.mouseButton.y, x, y, x + w, y + 16))
		{
			if (ev.mouseButton.button == sf::Mouse::Button::Right)
			{
				if (!waitingForEvent)
				{
					deviceName.setString("Waiting for input...");
					waitingForEvent = 1;
				}
			}
		}
	}
}

void LinkView::refresh()
{
	deviceName.setPosition(x + hPad, y + hPad);
}

void LinkView::remove()
{
	auto p = dynamic_cast<ParameterActionPanel*>(parent);
	if (p)
	{
		p->removeLink(std::pair<InputButton, Action>(*button, *action));
		p->requestRefresh();
	}
}