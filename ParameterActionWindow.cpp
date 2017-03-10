#include "ParameterActionWindow.h"


ParameterActionWindow::ParameterActionWindow(int w, int h, Parameter* param, std::vector<InputMap*> maps) : PopupWindow(w, h), p(param), inputMaps(maps)
{
	window.create(sf::VideoMode(128, 386), "ParameterActionWindow");
	texture = new sf::RenderTexture();
	texture->create(128, 386);
	//find all input/action pairs using p
	for (auto &m : inputMaps)
	{
		auto actions = m->findParameterActions(p);
		for (auto &a : actions)
		{
			addChild((std::make_unique<LinkView>(x, y, w, 16, a.first, a.second)));
		}
	}
}


ParameterActionWindow::~ParameterActionWindow()
{
	delete texture;
}

void ParameterActionWindow::update()
{
	sf::Event ev;
	while (window.pollEvent(ev))
	{
		if (ev.type == sf::Event::Closed)
		{
			quit = 1;
		}
		distributeEvent(ev);
	}

	



	PopupWindow::update();
	texture->clear();
}

void ParameterActionWindow::refresh()
{
	for (int i = 1; i < children.size(); i++)
	{
		int newY = children[i - 1]->getY() + children[i - 1]->getH();
		children[i]->setPosition(children[i]->getX(), newY);
	}
}

InputEvent ParameterActionWindow::detectNextEvent()
{
	InputButton input(InputButton::Device::None, 0);
	bool detected = 0;
	while (!detected)
	{
		sf::Event ev;
		while (window.pollEvent(ev))
		{
			if (ev.type == sf::Event::KeyPressed)
			{
				input.device = InputButton::Device::Keyboard;
				input.button = (int)ev.key.code;
				detected = 1;
				break;
			}
			if (ev.type == sf::Event::JoystickButtonPressed)
			{
				input.device = InputButton::Device::GamepadButton;
				input.button = (int)ev.joystickButton.button;
				detected = 1;
				break;
			}
			if (ev.type == sf::Event::JoystickMoved)
			{
				input.device = InputButton::Device::GamepadAxis;
				input.button = (int)ev.joystickMove.axis;
				detected = 1;
				break;
			}
		}
		//std::vector<unsigned char> message;
		//while (midiIn->isPortOpen())
		//{
		//	midiIn->getMessage(&message);
		//	if (message.empty())
		//		break;
		//	if (message[0] >= 144 && message[0] <= 159)//10010000 to 10011111 - note on
		//	{
		//		input.device = InputButton::Device::MIDINote;
		//		input.button = (int)message[1];
		//		detected = 1;
		//		break;
		//	}
		//	if (message[0] >= 176 && message[0] <= 191) //10110000 to 10111111 - control change
		//	{
		//		input.device = InputButton::Device::MIDICV;
		//		input.button = (int)message[1];
		//		detected = 1;
		//		break;
		//	}
		//}
	}
	return input;
}

void ParameterActionWindow::updateLink(std::pair<InputButton, Action> from, std::pair<InputButton, Action> to)
{
	for (auto &m : inputMaps)
		m->updateLink(from, to);
}