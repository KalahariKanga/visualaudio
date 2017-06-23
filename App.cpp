#include "App.h"
#include "ParameterView.h"
#include <fstream>

App::App()
{
	renderTexture[0].create(windowWidth, windowHeight);
	renderTexture[1].create(windowWidth, windowHeight);
	lastFrame.create(windowWidth, windowHeight);

	canvas = std::make_unique<Canvas>(windowWidth, windowHeight, &palette);
	midiIn = std::make_unique<RtMidiIn>();

	setupMidi();
	//midiIn->openPort(3);
	
	window.create(sf::VideoMode(windowWidth, windowHeight), "Window");
	//window.setVerticalSyncEnabled(true);

	ParameterView::popupCall = [this](Parameter* p){ requestParameterActionPanel(p); };
	LinkView::nextButton = [this](){ return eventHandler.nextButton(); };

	eventHandler.setInputMap(&inputMap);

	addParameter("scene", 0, 0, 16);

	shaderList.addShader("shaders/blend");
	//shaderList.addShader("shaders/tile");
	shaderList.addShader("shaders/polar");
	shaderList.addShader("shaders/kaleidoscope");
	shaderList.addShader("shaders/bend");
	shaderList.addShader("shaders/bloom");

	Action nextScene(getParameter("scene"), Action::Type::shift, 1);
	Action prevScene(getParameter("scene"), Action::Type::shift, -1);

	Action alpha(shaderList.getShader(0)->getParameter("alpha"), Action::Type::axis, 1);
	Action rotation(shaderList.getShader(0)->getParameter("angle"), Action::Type::axis, 1);
	Action zoom(shaderList.getShader(0)->getParameter("zoom"), Action::Type::axis, 1);

	inputMap.addAction(InputButton(InputButton::Device::Keyboard, (int)sf::Keyboard::Right), nextScene);
	inputMap.addAction(InputButton(InputButton::Device::Keyboard, (int)sf::Keyboard::Left), prevScene);

	inputMap.addAction(InputButton(InputButton::Device::GamepadAxis, 2), alpha);
	inputMap.addAction(InputButton(InputButton::Device::GamepadAxis, 0), zoom);
	inputMap.addAction(InputButton(InputButton::Device::GamepadAxis, 4), rotation);
	inputMap.addAction(InputButton(InputButton::Device::GamepadButton, 5), nextScene);
	inputMap.addAction(InputButton(InputButton::Device::GamepadButton, 4), prevScene);

	auto scene = addScene<Gen_Julia>();

	scene = addScene<Gen_Spirograph>();
	/*Action decay(scene->getParameter("decay"), Action::Type::axis, 1);
	Action burst(scene->getParameter("burst"), Action::Type::trigger);
	inputMap.addAction(InputButton(InputButton::Device::GamepadButton, 0), burst);*/

	scene = addScene<Gen_Particles>();
	Action outline(scene->getParameter("outline"), Action::Type::trigger);
	Action probability(scene->getParameter("spawnRate"), Action::Type::axis, 1);
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
	panel = std::make_unique<UIPanel>(0, 0, UIWidth, windowHeight, &shaderList,  scenes[0]->getGenerator(), &palette,&UITexture);

	//lock all scene parameters
	for (auto &s : scenes)
	{
		s->setParameterLock(true);
	}
}

App::~App()
{
}

void App::setupMidi()
{
	//just until I gui it
	int nPorts = midiIn->getPortCount();
	int port = 0;
	std::cout << "Found " << nPorts << " MIDI ports:\n";
	for (int c = 0; c < nPorts; c++)
		std::cout << c << ": " << midiIn->getPortName(c) << "\n";
	std::cin >> port;

	try
	{
		midiIn->openPort(port);
	}
	catch (...)
	{
		std::cout << "Cannot open MIDI port\n";
	}
}

void App::update()
{
	clock.restart();
	sceneID = (int)getParameter("scene")->getValue();
	if (sceneID >= scenes.size())
	{
		sceneID = scenes.size() - 1;
		getParameter("scene")->setValue(sceneID);
	}

	if (sceneID != lastSceneID)
		changeScene(sceneID);
	lastSceneID = sceneID;

	AC.update();
	palette.update();
	canvas->clear(sf::Color(0, 0, 0, 0));
	
	if (activeScene)
		activeScene->update(fps * lastFrameTime);//2 -> half fps -> move twice
	
	image.create(windowWidth, windowHeight, canvas->data);
	texture.create(windowWidth, windowHeight);
	texture.loadFromImage(image);
	sprite.setTexture(texture, 1);
	window.setView(sf::View(sf::FloatRect(0, 0, windowWidth, windowHeight)));

	applyShaders();

	if (showUI)
	{
		UITexture.clear(sf::Color(0, 0, 0, 128));
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
		if (subPanel.get())
			window.draw(sf::Sprite(UITexture.getTexture()));//this makes it like a billion times faster...? rather than panel->gettexture
		else
			window.draw(sf::Sprite(UITexture.getTexture(), sf::IntRect(0, 0, UIWidth, windowHeight)));
	}

	window.display();
	
	eventHandler.update();
	processEvents();

	while (clock.getElapsedTime().asSeconds() < 1.0 / fps)
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	lastFrameTime = clock.getElapsedTime().asSeconds();
}

void App::processEvents()
{
	sf::Event ev;
	eventHandler.addEvent(InputButton::Device::Audio, 0, AC.getAmplitude());
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
			case sf::Keyboard::S:
				save("savefile.txt");
				break;
			case sf::Keyboard::L:
				load("savefile.txt");
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
			eventHandler.addEvent(InputButton::Device::MIDINote, (int)message[1]);
		if (message[0] >= 176 && message[0] <= 191) //10110000 to 10111111 - control change
			eventHandler.addEvent(InputButton::Device::MIDICV, (int)message[1], (float)message[2] / 128);
	}
}

void App::applyShaders()
{
	//probably could be simpler
	if (shaderList.size() == 0) return;
	shaderList.getShader(0)->update();
	shaderList.getShader(0)->getShader()->setUniform("aspectRatio", (float)windowWidth / windowHeight);//omg :(
	shaderList.getShader(0)->getShader()->setUniform("lastFrame", lastFrame.getTexture());

	sprite.setScale(1, -1);
	sprite.setPosition(0, windowHeight);//dump on gpu

	if (shaderList.getShader(0)->isActive())
		renderTexture[0].draw(sprite, shaderList.getShader(0)->getShader());
	else
		renderTexture[0].draw(sprite);
	renderTexture[0].display();

	int t = 1;
	for (; t < shaderList.size(); t++)
	{
		shaderList.getShader(t)->update();
		shaderList.getShader(t)->getShader()->setUniform("aspectRatio", (float)windowWidth / windowHeight);
		shaderList.getShader(t)->getShader()->setUniform("lastFrame", lastFrame.getTexture());

		if (shaderList.getShader(t)->isActive())
			renderTexture[t % 2].draw(sf::Sprite(renderTexture[(t + 1) % 2].getTexture()), shaderList.getShader(t)->getShader());
		else
			renderTexture[t % 2].draw(sf::Sprite(renderTexture[(t + 1) % 2].getTexture()));
		renderTexture[t % 2].display();
	}
	window.draw(sf::Sprite(renderTexture[(t + 1) % 2].getTexture()));
	lastFrame.draw(sf::Sprite(renderTexture[(t + 1) % 2].getTexture()));
	lastFrame.display();
	renderTexture[0].clear();
}

void App::debugShaders()
{
	//dumb as heck, dont actually use
	static sf::RenderTexture temptex;
	static bool created = 0;
	if (!created)
	{
		temptex.create(windowWidth, windowHeight);
		created = 1;
	}
	if (shaderList.size() == 0) return;
	shaderList.getShader(0)->update();
	shaderList.getShader(0)->getShader()->setUniform("aspectRatio", (float)windowWidth / windowHeight);//omg :(
	shaderList.getShader(0)->getShader()->setUniform("lastFrame", lastFrame.getTexture());

	if (shaderList.getShader(0)->isActive())
	{
		sf::Sprite spr;
		spr.setTexture(renderTexture[0].getTexture());
		spr.setScale(0.16, 0.16);
		spr.setPosition(windowWidth*5.0/6.0, 0);
		temptex.draw(spr, shaderList.getShader(0)->getShader());
		renderTexture[0].draw(sprite, shaderList.getShader(0)->getShader());
	}
	else
	{
		sf::Sprite spr;
		spr.setTexture(renderTexture[0].getTexture());
		spr.setScale(0.16, 0.16);
		spr.setPosition(windowWidth*5.0 / 6.0, 0);
		temptex.draw(spr);
		renderTexture[0].draw(sprite);
	}

	renderTexture[0].display();
	int t = 1;
	for (; t < shaderList.size(); t++)
	{
		shaderList.getShader(t)->update();
		shaderList.getShader(t)->getShader()->setUniform("aspectRatio", (float)windowWidth / windowHeight);
		shaderList.getShader(t)->getShader()->setUniform("lastFrame", lastFrame.getTexture());

		if (shaderList.getShader(t)->isActive())
		{
			sf::Sprite spr;
			spr.setTexture(renderTexture[(t + 1) % 2].getTexture());
			spr.setScale(0.16, 0.16);
			spr.setPosition(windowWidth*5.0 / 6.0, t*windowHeight / 6.0);
			temptex.draw(spr, shaderList.getShader(t)->getShader());
			renderTexture[t % 2].draw(sf::Sprite(renderTexture[(t + 1) % 2].getTexture()), shaderList.getShader(t)->getShader());
		}
		else
		{
			sf::Sprite spr;
			spr.setTexture(renderTexture[(t + 1) % 2].getTexture());
			spr.setScale(0.16, 0.16);
			spr.setPosition(windowWidth*5.0 / 6.0, t*windowHeight / 6.0);
			temptex.draw(spr);
			renderTexture[t % 2].draw(sf::Sprite(renderTexture[(t + 1) % 2].getTexture()));
		}
		renderTexture[t % 2].display();
	}
	window.draw(sf::Sprite(renderTexture[(t + 1) % 2].getTexture()));
	window.draw(sf::Sprite(temptex.getTexture()));
	lastFrame.draw(sf::Sprite(renderTexture[(t + 1) % 2].getTexture()));
	lastFrame.display();
	temptex.display();
	renderTexture[0].clear();
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
	lastFrame.create(windowWidth, windowHeight);
	canvas->resize(windowWidth, windowHeight);
	UITexture.create(UIWidth * 2, windowHeight);
}

void App::initialize()
{
	canvas->clear(sf::Color(0, 0, 0, 255));
	scenes.clear();
	activeScene = 0;
	getParameter("scene")->setValue(0);
	//inputMap.clear();//impl
	//eventHandler.clear();//impl
	shaderList.clear();

	changeScene(-1);
}

void App::changeScene(int id)
{
	if (activeScene)
	{
		activeScene->setParameterLock(true);
	}

	if (id < 0 || id >= scenes.size())
	{
		activeScene = nullptr;
		panel = std::make_unique<UIPanel>(0, 0, UIWidth, windowHeight, &shaderList, nullptr, &palette, &UITexture);
	}
	else
	{
		activeScene = scenes[id].get();//try
		activeScene->setParameterLock(false);
		panel = std::make_unique<UIPanel>(0, 0, UIWidth, windowHeight, &shaderList, activeScene->getGenerator(), &palette, &UITexture);

	}

	panel->doRefresh();
	subPanel.reset(nullptr);//hide panel on scene change
}

void App::requestParameterActionPanel(Parameter* param)
{
	subPanel = std::make_unique<ParameterActionPanel>(UIWidth, windowHeight, param, &inputMap, &UITexture);
}

void App::save(std::string fname)
{
	std::ofstream file(fname, std::ios::trunc);
	std::cout << "Saving to file " << fname << "...\n";
	if (!file.is_open())
	{
		std::cout << "Could not open file " << fname << "\n";
		return;
	}

	file << scenes.size() << std::endl;
	
	for (auto &sc : scenes)
	{
		file << sc->getGenerator()->getName() << std::endl;
		auto genParamList = sc->getGenerator()->getParameterList();
		file << genParamList.size() << std::endl;
		for (auto &p : genParamList)
		{
			file << p->getName() << std::endl;
			file << p->getValue() << std::endl;
		}

	}

	int sceneID = (int)getParameter("scene")->getValue();
	file << sceneID << std::endl;

	file << shaderList.size() << std::endl;
	
	for (int c = 0; c < shaderList.size(); c++)
	{
		auto sh = shaderList.getShader(c);
		file << sh->getName() << std::endl;
		file << sh->isActive() << std::endl;
		auto shaderParamList = sh->getParameterList();
		file << shaderParamList.size() << std::endl;
		for (auto &p : shaderParamList)
		{
			file << p->getName() << std::endl;
			file << p->getValue() << std::endl;
		}
	}

	auto paletteParamList = palette.getParameterList();
	file << paletteParamList.size() << std::endl;
	for (auto &p : paletteParamList)
	{
		file << p->getName() << std::endl;
		file << p->getValue() << std::endl;
	}
}

void App::load(std::string fname)
{
	std::ifstream file(fname);
	std::cout << "Loading file " << fname << "...\n";
	if (!file.is_open())
	{
		std::cout << "Could not open file.\n";
		return;
	}

	initialize();

	int noScenes = 0;
	file >> noScenes;
	for (int c = 0; c < noScenes; c++)
	{
		Scene* sc = nullptr;
		std::string scenetype;
		file >> scenetype;//
		sc = addScene(scenetype);
		int noParams = 0;
		file >> noParams;
		for (int d = 0; d < noParams; d++)
		{
			std::string name;
			float val = 0;
			file >> name;//
			file >> val;
			sc->getParameter(name)->setValue(val);//try
		}
	}

	file >> sceneID;
	lastSceneID = -1;//force scene switch
	getParameter("scene")->setValue(sceneID);

	int noShaders = 0;
	file >> noShaders;
	for (int c = 0; c < noShaders; c++)
	{
		std::string name;
		bool active;
		file >> name;
		file >> active;
		shaderList.addShader(name);//try
		auto sh = shaderList.getShader(c);
		sh->setActive(active);

		int noParams = 0;
		file >> noParams;
		for (int d = 0; d < noParams; d++)
		{
			std::string name;
			float val = 0;
			file >> name;//
			file >> val;
			sh->getParameter(name)->setValue(val);//try
		}
	}

	int noParams = 0;
	file >> noParams;
	for (int c = 0; c < noParams; c++)
	{
		std::string name;
		float val = 0;
		file >> name;//
		file >> val;
		palette.getParameter(name)->setValue(val);//try
	}

}

Scene* App::addScene(std::string sceneType)
{
	if (sceneType == "CircleSpectrum")
		return addScene<Gen_CircleSpectrum>();
	else if (sceneType == "Julia")
		return addScene<Gen_Julia>();
	else if (sceneType == "Particles")
		return addScene<Gen_Particles>();
	else if (sceneType == "Spectrum")
		return addScene<Gen_Spectrum>();
	else if (sceneType == "Spirograph")
		return addScene<Gen_Spirograph>();
	else if (sceneType == "Swarm")
		return addScene<Gen_Swarm>();
	else if (sceneType == "Waveform")
		return addScene<Gen_Waveform>();
	else
		return nullptr;
}