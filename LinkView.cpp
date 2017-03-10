#include "LinkView.h"
#include "ActionView.h"
#include "ParameterActionWindow.h"

LinkView::LinkView(int x, int y, int w, int h, InputButton button, Action action) : UIElement(x, y, w, h), button(button), action(action)
{
	deviceName.setString(button.getDeviceName()+" "+std::to_string((int)button.button));
	deviceName.setFillColor(sf::Color::White);
	deviceName.setFont(*UIElement::getFont());
	deviceName.setCharacterSize(12);

	addChild(std::make_unique<ActionView>(x, y + 16, w, 48, action));
}


LinkView::~LinkView()
{
}

void LinkView::update()
{
	draw(deviceName);
	//draw(actionTarget);
}

void LinkView::processEvent(sf::Event ev)
{
	if (waitingForEvent)
	{
		auto newButton = sfEventToInputButton(ev);
		if (newButton.device != InputButton::Device::None)
		{
			waitingForEvent = 0;
			deviceName.setString(newButton.getDeviceName() + " " + std::to_string((int)newButton.button));
			auto p = dynamic_cast<ParameterActionWindow*>(parent);
			if (p)
			{
				p->updateLink(std::make_pair(button, action), std::make_pair(newButton, action));
			}
			else
				throw std::exception("LinkView's parent must be a ParameterActionWindow!");
			button = newButton;
			return;
		}
	}
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