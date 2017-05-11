#include "LinkView.h"
#include "ActionView.h"
#include "ParameterActionWindow.h"

std::function<InputButton(void)> LinkView::nextButton;

LinkView::LinkView(int x, int y, int w, int h, InputButton* button, Action* action) : UIElement(x, y, w, h), button(button), action(action)
{
	deviceName.setString(button->getDeviceName() + " " + std::to_string((int)button->button));
	deviceName.setFillColor(sf::Color::White);
	deviceName.setFont(*UIElement::getFont());
	deviceName.setCharacterSize(12);

	addChild<ActionView>(x, y + 16, w, 48, action);
	addChild<UIButton>(x + w - 16, y, 8, 8, [&](){remove(); }, "x");
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
	deviceName.setPosition(x + 4, y + 4);
}

InputButton LinkView::sfEventToInputButton(sf::Event ev)
{
	InputButton input(InputButton::Device::None, 0);
	if (ev.type == sf::Event::JoystickMoved && abs(ev.joystickMove.position) > 20)//deadzone
	{
		std::cout << ev.joystickMove.position << " ";
		input.device = InputButton::Device::GamepadAxis;
		input.button = (int)ev.joystickMove.axis;
	}
	if (ev.type == sf::Event::KeyPressed)
	{
		input.device = InputButton::Device::Keyboard;
		input.button = (int)ev.key.code;
	}
	if (ev.type == sf::Event::JoystickButtonPressed)
	{
		input.device = InputButton::Device::GamepadButton;
		input.button = (int)ev.joystickButton.button;
	}
	
	return input;
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