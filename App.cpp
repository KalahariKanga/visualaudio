#include "App.h"
#include "ParameterView.h"
#include <fstream>

App::App()
{
	window.create(sf::VideoMode(windowWidth, windowHeight), "Window");
	//window.setVerticalSyncEnabled(true);
	renderTexture[0].create(windowWidth, windowHeight);
	renderTexture[1].create(windowWidth, windowHeight);

	canvas = std::make_unique<Canvas>(windowWidth, windowHeight, &palette);
	midiIn = std::make_unique<RtMidiIn>();
	try
	{
		midiIn->openPort(3);
	}
	catch (...)
	{
		std::cout << "Cannot open MIDI port\n";
	}

	ParameterView::popupCall = [this](Parameter* p){ requestParameterActionPanel(p); };
	LinkView::nextButton = [this](){ return eventHandler.nextButton(); };

	eventHandler.setInputMap(&inputMap);

	addParameter("scene", 0, 0, 16);

	shaderList.addShader("shaders/blend");
	shaderList.getShader(0)->getShader()->setUniform("lastFrame", renderTexture[0].getTexture());
	/*shaderList.addShader("shaders/tile");
	shaderList.addShader("shaders/mosaic");*/
	shaderList.addShader("shaders/kaleidoscope");
	shaderList.addShader("shaders/bend");
	shaderList.addShader("shaders/bloom");

	Action nextScene(getParameter("scene"), Action::Type::shift, 1);
	Action prevScene(getParameter("scene"), Action::Type::shift, -1);

	Action alpha(shaderList.getShader(0)->getParameter("alpha"), Action::Type::axis, 1);
	Action rotation(shaderList.getShader(0)->getParameter("angle"), Action::Type::axis, 1);
	Action zoom(shaderList.getShader(0)->getParameter("zoom"), Action::Type::axis, 1);

	Action moreMirrors(shaderList.getShader(1)->getParameter("reflections"), Action::Type::shift, 1);
	Action lessMirrors(shaderList.getShader(1)->getParameter("reflections"), Action::Type::shift, -1);
	Action kalUp(shaderList.getShader(1)->getParameter("ypos"), Action::Type::shift, 0.1);
	Action kalDown(shaderList.getShader(1)->getParameter("ypos"), Action::Type::shift, -0.1);
	Action kalLeft(shaderList.getShader(1)->getParameter("xpos"), Action::Type::shift, 0.1);
	Action kalRight(shaderList.getShader(1)->getParameter("xpos"), Action::Type::shift, -0.1);
	Action flip(shaderList.getShader(1)->getParameter("flip"), Action::Type::trigger);
	
	//for (int c = 0; c < 9; c++)
	//{
	//	auto a = Action(shaders[0]->getParameter("alpha"), Action::Type::setNormalised, (float)c / 9);
	//	eventHandler.addAction(InputButton(InputButton::Device::MIDINote, c), a);
	//	a = Action(shaders[0]->getParameter("zoom"), Action::Type::setNormalised, (float)c / 9);
	//	eventHandler.addAction(InputButton(InputButton::Device::MIDINote, c+16), a);
	//	a = Action(shaders[0]->getParameter("angle"), Action::Type::setNormalised, (float)c / 9);
	//	eventHandler.addAction(InputButton(InputButton::Device::MIDINote, c+32), a);
	//}

	inputMap.addAction(InputButton(InputButton::Device::Keyboard, (int)sf::Keyboard::Right), nextScene);
	inputMap.addAction(InputButton(InputButton::Device::Keyboard, (int)sf::Keyboard::Left), prevScene);

	inputMap.addAction(InputButton(InputButton::Device::GamepadAxis, 2), alpha);
	inputMap.addAction(InputButton(InputButton::Device::GamepadAxis, 0), zoom);
	inputMap.addAction(InputButton(InputButton::Device::GamepadAxis, 4), rotation);
	inputMap.addAction(InputButton(InputButton::Device::GamepadButton, 5), nextScene);
	inputMap.addAction(InputButton(InputButton::Device::GamepadButton, 4), prevScene);

	inputMap.addAction(InputButton(InputButton::Device::MIDICV, 74), alpha);
	inputMap.addAction(InputButton(InputButton::Device::MIDICV, 71), zoom);
	inputMap.addAction(InputButton(InputButton::Device::MIDICV, 81), rotation);

	inputMap.addAction(InputButton(InputButton::Device::Keyboard, (int)sf::Keyboard::Up), moreMirrors);
	inputMap.addAction(InputButton(InputButton::Device::Keyboard, (int)sf::Keyboard::Down), lessMirrors);
	inputMap.addAction(InputButton(InputButton::Device::Keyboard, (int)sf::Keyboard::W), kalUp);
	inputMap.addAction(InputButton(InputButton::Device::Keyboard, (int)sf::Keyboard::S), kalDown);
	inputMap.addAction(InputButton(InputButton::Device::Keyboard, (int)sf::Keyboard::A), kalLeft);
	inputMap.addAction(InputButton(InputButton::Device::Keyboard, (int)sf::Keyboard::D), kalRight);
	inputMap.addAction(InputButton(InputButton::Device::Keyboard, (int)sf::Keyboard::F), flip);

	//eventHandler.addAction(InputButton(InputButton::Device::Audio, 0), rotation);

	auto scene = addScene<Gen_Julia>();


	scene = addScene<Gen_Spirograph>();
	Action decay(scene->getParameter("decay"), Action::Type::axis, 1);
	Action burst(scene->getParameter("burst"), Action::Type::trigger);
	inputMap.addAction(InputButton(InputButton::Device::GamepadButton, 0), burst);
	

	scene = addScene<Gen_Particles>();
	Action outline(scene->getParameter("outline"), Action::Type::trigger);
	Action probability(scene->getParameter("spawnProbability"), Action::Type::axis, 1);
	Action reverse(scene->getParameter("reverse"), Action::Type::trigger);
	Action split(scene->getParameter("split"), Action::Type::trigger);
	inputMap.addAction(InputButton(InputButton::Device::GamepadButton, 0), outline);
	inputMap.addAction(InputButton(InputButton::Device::GamepadAxis, 1), probability);
	inputMap.addAction(InputButton(InputButton::Device::GamepadButton, 1), reverse);
	inputMap.addAction(InputButton(InputButton::Device::GamepadButton, 2), split);

	scene = addScene<Gen_Swarm>();
	Action moreParticles(scene->getParameter("noParts"), Action::Type::shift, 5);
	Action fewerParticles(scene->getParameter("noParts"), Action::Type::shift, -5);
	inputMap.addAction(InputButton(InputButton::Device::MIDINote, 0), moreParticles);
	inputMap.addAction(InputButton(InputButton::Device::MIDINote, 1), fewerParticles);

	scene = addScene<Gen_Waveform>();
	Action fill(scene->getParameter("fill"), Action::Type::trigger);
	inputMap.addAction(InputButton(InputButton::Device::GamepadButton, 0), fill);

	addScene<Gen_Spectrum>();
	addScene<Gen_CircleSpectrum>();

	UITexture.create(UIWidth * 2, windowHeight);
	panel = std::make_unique<UIPanel>(0, 0, UIWidth, windowHeight, &shaderList, scenes[0]->getGenerator(), &UITexture);

	//lock all scene parameters
	for (auto &s : scenes)
	{
		s->setParameterLock(true);
	}
}


App::~App()
{
}

void App::update()
{
	clock.restart();
	int sceneID = (int)getParameter("scene")->getValue();
	if (sceneID >= scenes.size())
	{
		sceneID = scenes.size() - 1;
		getParameter("scene")->setValue(sceneID);
	}

	if (activeScene != scenes[sceneID].get())
	{
		if (activeScene)
		{
			activeScene->setParameterLock(true);
		}
		activeScene = scenes[sceneID].get();//try
		activeScene->setParameterLock(false);
		panel = std::make_unique<UIPanel>(0, 0, UIWidth, windowHeight, &shaderList, activeScene->getGenerator(), &UITexture);
		panel->doRefresh();
	}

	AC.update();
	palette.update();
	canvas->clear(sf::Color(0, 0, 0, 0));
	
	activeScene->update(fps * lastFrameTime);//2 -> half fps -> move twice
	

	image.create(windowWidth, windowHeight, canvas->data);
	texture.create(windowWidth, windowHeight);
	texture.loadFromImage(image);
	sprite.setTexture(texture, 1);
	window.setView(sf::View(sf::FloatRect(0, 0, windowWidth, windowHeight)));

	applyShaders();

	if (showUI)
	{
		UITexture.clear(sf::Color(0, 0, 0, 0));
		panel->doUpdate();
		if (subPanel.get())
		{
			subPanel->doRefresh();
			subPanel->doUpdate();
			if (subPanel->toQuit)
			{
				subPanel.reset(nullptr);
			}
		}

		UITexture.display();

		window.draw(sf::Sprite(UITexture.getTexture()));//this makes it like a billion times faster...? rather than panel->gettexture

	}

	window.display();
	
	eventHandler.update();
	processEvents();

	//std::cout << 1/clock.getElapsedTime().asSeconds() << "\n";

	while (clock.getElapsedTime().asSeconds() < 1.0 / fps)
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	lastFrameTime = clock.getElapsedTime().asSeconds();
}

void App::processEvents()
{
	sf::Event ev;
	eventHandler.addEvent(InputButton::Device::Audio, 0, AC.getAmplitude()/10);
	while (window.pollEvent(ev))
	{
		if (ev.type == sf::Event::Closed)
			quit = 1;
		if (ev.type == sf::Event::Resized)
		{
			resize(ev.size.width, ev.size.height);
			break;//avoid double delete on snap resize
		}
		if (ev.type == sf::Event::KeyPressed)
		{
			switch (ev.key.code)
			{
			case sf::Keyboard::N:
				AC.normalise();
				break;
			case sf::Keyboard::R:
				panel->doRefresh();
				break;
			case sf::Keyboard::Tab:
				showUI = !showUI;
				break;
			case sf::Keyboard::F4:
				toggleFullscreen();
				break;
			case sf::Keyboard::Escape:
				quit = 1;
				break;
			}
			eventHandler.addEvent(InputButton::Device::Keyboard, (int)ev.key.code);
		}
		if (ev.type == sf::Event::JoystickButtonPressed)
		{
			eventHandler.addEvent(InputButton::Device::GamepadButton, (int)ev.joystickButton.button);
		}
		if (ev.type == sf::Event::JoystickMoved)
		{
			eventHandler.addEvent(InputButton::Device::GamepadAxis, (int)ev.joystickMove.axis, ev.joystickMove.position / 200 + 0.5);
		}
		if (showUI)
		{
			panel->distributeEvent(ev);
			if (subPanel.get())
				subPanel->distributeEvent(ev);
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
			eventHandler.addEvent(InputButton::Device::MIDINote, (int)message[1]);
			std::cout << (int)message[1] << "\n";
		}
		if (message[0] >= 176 && message[0] <= 191) //10110000 to 10111111 - control change
		{
			eventHandler.addEvent(InputButton::Device::MIDICV, (int)message[1], (float)message[2] / 128);
			std::cout << (int)message[1] << "\n";
		}
	}
}

void App::applyShaders()
{
	//probably could be simpler
	if (shaderList.size() == 0) return;
	shaderList.getShader(0)->update();
	if (shaderList.getShader(0)->isActive())
		renderTexture[0].draw(sprite, shaderList.getShader(0)->getShader());
	else
		renderTexture[0].draw(sprite);

	renderTexture[0].display();
	int t = 1;
	for (; t < shaderList.size(); t++)
	{
		shaderList.getShader(t)->update();
		if (shaderList.getShader(t)->isActive())
			renderTexture[t % 2].draw(sf::Sprite(renderTexture[(t + 1) % 2].getTexture()), shaderList.getShader(t)->getShader());
		else
			renderTexture[t % 2].draw(sf::Sprite(renderTexture[(t + 1) % 2].getTexture()));
		renderTexture[t % 2].display();
	}
	window.draw(sf::Sprite(renderTexture[(t+1) % 2].getTexture()));
}

InputButton App::detectNextInput()
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
		std::vector<unsigned char> message;
		while (midiIn->isPortOpen())
		{
			midiIn->getMessage(&message);
			if (message.empty())
				break;
			if (message[0] >= 144 && message[0] <= 159)//10010000 to 10011111 - note on
			{
				input.device = InputButton::Device::MIDINote;
				input.button = (int)message[1];
				detected = 1;
				break;
			}
			if (message[0] >= 176 && message[0] <= 191) //10110000 to 10111111 - control change
			{
				input.device = InputButton::Device::MIDICV;
				input.button = (int)message[1];
				detected = 1;
				break;
			}
		}
	}
	std::cout << (int)input.device << ": " << input.button << "\n";
	return input;
}

void App::toggleFullscreen()
{
	if (fullscreen)
	{
		fullscreen = 0;
		resize(800, 600);
		window.create(sf::VideoMode(800, 600), "Window");
	}
	else
	{
		fullscreen = 1;
		auto mode = sf::VideoMode::getFullscreenModes().front();
		resize(mode.width, mode.height);
		window.create(mode, "Window", sf::Style::Fullscreen);
	}
}

void App::resize(int width, int height)
{
	windowWidth = width;
	windowHeight = height;
	renderTexture[0].create(windowWidth, windowHeight);
	renderTexture[1].create(windowWidth, windowHeight);
	canvas->resize(windowWidth, windowHeight);
	UITexture.create(UIWidth * 2, windowHeight);
}

void App::requestParameterActionPanel(Parameter* param)
{
	subPanel = std::make_unique<ParameterActionPanel>(192, windowHeight, param, &inputMap, &UITexture);
}