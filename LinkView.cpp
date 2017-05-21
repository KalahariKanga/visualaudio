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
	addChild<UIButton>(x + w - buttonSize - hPad, y,				buttonSize,	buttonSize, [&](){ remove(); }, "x");
	addChild<UIButton>(x + w - buttonSize - hPad, y + buttonSize,	buttonSize, buttonSize, [&](){ linkAudio(); }, "~");

	children.back()->setActive(0);
}


LinkView::~LinkView()
{
}

void LinkView::update()
{
	children.back()->setActive(waitingForEvent);//eww
	if (waitingForEvent)
	{
		auto newButton = nextButton();//get next event
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
		//get input on left click
		if (Math::pointInRect(ev.mouseButton.x, ev.mouseButton.y, x, y, x + w, y + hStep))
		{
			if (ev.mouseButton.button == sf::Mouse::Button::Left)
			{
				if (!waitingForEvent)
				{
					deviceName.setString("Waiting for input...");
					waitingForEvent = 1;
				}
			}
		}
	}
	//highlight
	if (ev.type == sf::Event::MouseMoved)
	{
		if (Math::pointInRect(ev.mouseMove.x, ev.mouseMove.y, x, y, x + w, y + hStep) || waitingForEvent)
			deviceName.setFillColor(UIStyle::Colour::PrimaryHighlight);
		else
			deviceName.setFillColor(UIStyle::Colour::Primary);
	}
}

void LinkView::refresh()
{
	deviceName.setPosition(x + hPad, y);
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

void LinkView::linkAudio()
{
	InputButton newButton = InputButton(InputButton::Device::Audio, 0);
	waitingForEvent = 0;
	deviceName.setString(newButton.getDeviceName() + " " + std::to_string((int)newButton.button));
	*button = newButton;
}