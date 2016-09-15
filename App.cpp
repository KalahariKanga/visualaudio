#include "App.h"


App::App()
{
	window.create(sf::VideoMode(windowWidth, windowHeight), "Window");
	lastFrame.create(windowWidth, windowHeight);//!!!!
	lastFrame.clear();

	canvas = std::make_unique<Canvas>(windowWidth, windowHeight);
	midiIn = std::make_unique<RtMidiIn>();
	try
	{
		midiIn->openPort(3);
	}
	catch (...)
	{
		std::cout << "Cannot open MIDI port\n";
	}

	
	addParameter("scene", 0, 0, 16, 1);
	addParameter("driftX", 0, -0.1, 0.1);
	addParameter("driftY", 0, -0.1, 0.1);
	addParameter("zoom", 1, 0.8, 1.2);
	addParameter("angle", 0, -0.4, 0.4);
	blendShader.loadFromFile("shaders/blend", sf::Shader::Fragment);
	blendShader.setParameter("lastFrame", lastFrame.getTexture());
	
	shader.loadFromFile("shaders/bloom", sf::Shader::Fragment);
	shader.setParameter("size_f", 4);
	//shader.setParameter("viewAngle", PI / 5);

	Action nextScene(getParameter("scene"), Action::Type::shift, 1);
	Action prevScene(getParameter("scene"), Action::Type::shift, -1);
	Action alpha(canvas->getParameter("clearAlpha"), Action::Type::axis, 1);
	Action rotation(getParameter("angle"), Action::Type::axis, 1);
	Action zoom(getParameter("zoom"), Action::Type::axis, 1);
	
	for (int c = 0; c < 9; c++)
	{
		auto a = Action(canvas->getParameter("clearAlpha"), Action::Type::setNormalised, (float)c / 9);
		eventHandler.addAction(InputButton(InputButton::Device::MIDI, c), a);
		a = Action(getParameter("zoom"), Action::Type::setNormalised, (float)c / 9);
		eventHandler.addAction(InputButton(InputButton::Device::MIDI, c+16), a);
		a = Action(getParameter("angle"), Action::Type::setNormalised, (float)c / 9);
		eventHandler.addAction(InputButton(InputButton::Device::MIDI, c+32), a);
	}

	eventHandler.addAction(InputButton(InputButton::Device::GamepadAxis, 2), alpha);
	eventHandler.addAction(InputButton(InputButton::Device::GamepadAxis, 0), zoom);
	eventHandler.addAction(InputButton(InputButton::Device::GamepadAxis, 4), rotation);
	eventHandler.addAction(InputButton(InputButton::Device::GamepadButton, 5), nextScene);
	eventHandler.addAction(InputButton(InputButton::Device::GamepadButton, 4), prevScene);

	auto scene = addScene<Gen_CircleSpectrum>();
	Action decay(scene->getParameter("decay"), Action::Type::axis, 1);
	Action bandsUp(scene->getParameter("bands"), Action::Type::shift, 1);
	Action bandsDown(scene->getParameter("bands"), Action::Type::shift, -1);
	scene->addAction(InputButton(InputButton::Device::GamepadButton, 0), bandsUp);
	scene->addAction(InputButton(InputButton::Device::GamepadButton, 1), bandsDown);
	

	scene = addScene<Gen_Particles>();
	Action outline(scene->getParameter("outline"), Action::Type::trigger);
	Action dirChange(scene->getParameter("directionChange"), Action::Type::axis, 1);
	scene->addAction(InputButton(InputButton::Device::GamepadButton, 0), outline);
	scene->addAction(InputButton(InputButton::Device::GamepadAxis, 1), dirChange);

	scene = addScene<Gen_Swarm>();
	Action moreParticles(scene->getParameter("noParts"), Action::Type::shift, 5);
	Action fewerParticles(scene->getParameter("noParts"), Action::Type::shift, -5);
	scene->addAction(InputButton(InputButton::Device::MIDI, 0), moreParticles);
	scene->addAction(InputButton(InputButton::Device::MIDI, 1), fewerParticles);
}


App::~App()
{
}

void App::update()
{

	int sceneID = (int)getParameter("scene")->getValue();
	if (sceneID >= scenes.size())
	{
		sceneID = scenes.size() - 1;
		getParameter("scene")->setValue(sceneID);
	}
	activeScene = scenes[sceneID].get();//try

	sf::Event ev;
	clock.restart();
	AC.update();
	canvas->update();

	if (1)
		canvas->clear(sf::Color(0, 0, 0, 0));
	else
		canvas->wipe();//slow

	activeScene->update();
	eventHandler.update();

	image.create(windowWidth, windowHeight, canvas->data);
	texture.loadFromImage(image);
	sprite.setTexture(texture);
	window.setView(sf::View(sf::FloatRect(0, 0, windowWidth, windowHeight)));

	if (1)
	{
		blendShader.setParameter("alpha", canvas->getParameter("clearAlpha")->getValue());
		blendShader.setParameter("drift", sf::Vector2f(getParameter("driftX")->getValue(),getParameter("driftY")->getValue()));
		blendShader.setParameter("zoom", getParameter("zoom")->getValue());
		blendShader.setParameter("angle", getParameter("angle")->getValue());
		lastFrame.draw(sprite, &blendShader);
		lastFrame.display();
		window.draw(sf::Sprite(lastFrame.getTexture()), &shader);
	}
	else
	{
		window.draw(sprite, &shader);
	}

	window.display();

	while (window.pollEvent(ev))
	{
		if (ev.type == sf::Event::Closed)
			quit = 1;
		if (ev.type == sf::Event::Resized)
		{
			windowWidth = ev.size.width;
			windowHeight = ev.size.height;
			canvas->resize(windowWidth, windowHeight);
		}
		if (ev.type == sf::Event::KeyPressed)
		{
			switch (ev.key.code)
			{
			case sf::Keyboard::N:
				AC.normalise();
				break;
			}
			activeScene->addEvent(InputButton::Device::Keyboard, (int)ev.key.code);
			eventHandler.addEvent(InputButton::Device::Keyboard, (int)ev.key.code);
		}
		if (ev.type == sf::Event::JoystickButtonPressed)
		{
			activeScene->addEvent(InputButton::Device::GamepadButton, (int)ev.joystickButton.button);
			eventHandler.addEvent(InputButton::Device::GamepadButton, (int)ev.joystickButton.button);
		}
		if (ev.type == sf::Event::JoystickMoved)
		{
			activeScene->addEvent(InputButton::Device::GamepadAxis, (int)ev.joystickMove.axis, ev.joystickMove.position / 200 + 0.5);
			eventHandler.addEvent(InputButton::Device::GamepadAxis, (int)ev.joystickMove.axis, ev.joystickMove.position / 200 + 0.5);
		}
	}
	std::vector<unsigned char> message;
	while (midiIn->isPortOpen())
	{
		midiIn->getMessage(&message);
		if (message.empty())
			break;
		if (message[0] >= 144 && message[0] <= 159)//10010000 to 10011111 - note on
		{
			activeScene->addEvent(InputButton::Device::MIDI, (int)message[1]);
			eventHandler.addEvent(InputButton::Device::MIDI, (int)message[1]);
			std::cout << (int)message[1] << "\n";
		}
	}
	
	//std::cout << clock.getElapsedTime().asSeconds() << "\n";
	while (clock.getElapsedTime().asSeconds() < 1.0 / fps)
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

