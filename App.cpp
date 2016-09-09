#include "App.h"


App::App()
{
	window.create(sf::VideoMode(windowWidth, windowHeight), "Window");
	lastFrame.create(windowWidth, windowHeight);//!!!!
	lastFrame.clear();

	canvas = std::make_unique<Canvas>(windowWidth, windowHeight);
	
	
	addParameter("scene", 0, 0, 16, 1);

	blendShader.loadFromFile("shaders/blend", sf::Shader::Fragment);
	blendShader.setParameter("lastFrame", lastFrame.getTexture());
	
	shader.loadFromFile("shaders/bloom", sf::Shader::Fragment);
	shader.setParameter("size_f", 4);
	//shader.setParameter("viewAngle", PI / 5);

	Action nextScene(getParameter("scene"), Action::Type::shift, 1);
	Action prevScene(getParameter("scene"), Action::Type::shift, -1);
	Action alpha(canvas->getParameter("clearAlpha"), Action::Type::set, 1);

	auto scene = addScene<Gen_CircleSpectrum>();
	Action decay(scene->getParameter("decay"), Action::Type::set, 1);
	Action bandsUp(scene->getParameter("bands"), Action::Type::shift, 1);
	Action bandsDown(scene->getParameter("bands"), Action::Type::shift, -1);
	scene->addAction(InputButton(InputButton::Device::GamepadButton, 5), nextScene);
	scene->addAction(InputButton(InputButton::Device::GamepadButton, 4), prevScene);
	scene->addAction(InputButton(InputButton::Device::GamepadButton, 0), bandsUp);
	scene->addAction(InputButton(InputButton::Device::GamepadButton, 1), bandsDown);
	scene->addAction(InputButton(InputButton::Device::GamepadAxis, 2), alpha);
	scene = addScene<Gen_Waveform>();
	scene->addAction(InputButton(InputButton::Device::GamepadButton, 5), nextScene);
	scene->addAction(InputButton(InputButton::Device::GamepadButton, 4), prevScene);
	scene = addScene<Gen_Spectrum>();
	scene->addAction(InputButton(InputButton::Device::GamepadButton, 5), nextScene);
	scene->addAction(InputButton(InputButton::Device::GamepadButton, 4), prevScene);

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

	image.create(windowWidth, windowHeight, canvas->data);
	texture.loadFromImage(image);
	sprite.setTexture(texture);
	window.setView(sf::View(sf::FloatRect(0, 0, windowWidth, windowHeight)));

	if (1)
	{
		blendShader.setParameter("alpha", canvas->getParameter("clearAlpha")->getValue());
		blendShader.setParameter("drift", sf::Vector2f(0, 0));
		blendShader.setParameter("zoom", 0.9);
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
		}
		if (ev.type == sf::Event::JoystickButtonPressed)
		{
			activeScene->addEvent(InputButton::Device::GamepadButton, (int)ev.joystickButton.button);
		}
		if (ev.type == sf::Event::JoystickMoved)
		{
			activeScene->addEvent(InputButton::Device::GamepadAxis, (int)ev.joystickMove.axis, ev.joystickMove.position / 100);
		}
	}
	
	while (clock.getElapsedTime().asSeconds() < 1.0 / fps)
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

