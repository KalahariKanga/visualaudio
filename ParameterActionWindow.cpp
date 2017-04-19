#include "ParameterActionWindow.h"


ParameterActionWindow::ParameterActionWindow(int w, int h, Parameter* param, InputMap* map) : PopupWindow(w, h), p(param), inputMap(map)
{
	window.create(sf::VideoMode(128, 386), "ParameterActionWindow");
	texture = new sf::RenderTexture();
	texture->create(128, 386);

	title.setString(param->getName());
	title.setFillColor(sf::Color::White);
	title.setFont(*UIElement::getFont());
	title.setCharacterSize(12);



	//find all input/action pairs using p

	auto actions = inputMap->findParameterActions(p);
	for (auto &a : actions)
	{
		addChild((std::make_unique<LinkView>(x, y+20, w, 16, a.first, a.second)));
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

	
	draw(title);


	PopupWindow::update();
	texture->clear();
}

void ParameterActionWindow::processEvent(sf::Event ev)
{
	if (ev.type == sf::Event::KeyPressed)
	{
		if (ev.key.code == sf::Keyboard::Space)
		{
			inputMap->addAction(InputButton(InputButton::Device::None, -1), Action(p, Action::Type::set));
			rebuildChildren();
		}
	}
}

void ParameterActionWindow::rebuildChildren()
{
	children.clear();
	auto actions = inputMap->findParameterActions(p);
	for (auto &a : actions)
	{
		addChild((std::make_unique<LinkView>(x, y + 20, w, 16, a.first, a.second)));
	}
}

void ParameterActionWindow::refresh()
{
	title.setPosition(x + 4, y + 4);
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