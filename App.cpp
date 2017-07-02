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
	UIComboBox::popupCall = [this](UIComboBox* box){ requestComboBoxPanel(box); };

	eventHandler.setInputMap(&inputMap);

	addParameter("scene", 0, 0, 16);

	//shaderList.addShader("shaders/blend");

	Action nextScene(getParameter("scene"), Action::Type::shift, 1);
	Action prevScene(getParameter("scene"), Action::Type::shift, -1);

	/*Action alpha(shaderList.getShader(0)->getParameter("alpha"), Action::Type::axis, 1);
	Action rotation(shaderList.getShader(0)->getParameter("angle"), Action::Type::axis, 1);
	Action zoom(shaderList.getShader(0)->getParameter("zoom"), Action::Type::axis, 1);*/

	inputMap.addAction(InputButton(InputButton::Device::Keyboard, (int)sf::Keyboard::Right), nextScene);
	inputMap.addAction(InputButton(InputButton::Device::Keyboard, (int)sf::Keyboard::Left), prevScene);

	/*inputMap.addAction(InputButton(InputButton::Device::GamepadAxis, 2), alpha);
	inputMap.addAction(InputButton(InputButton::Device::GamepadAxis, 0), zoom);
	inputMap.addAction(InputButton(InputButton::Device::GamepadAxis, 4), rotation);*/
	inputMap.addAction(InputButton(InputButton::Device::GamepadButton, 5), nextScene);
	inputMap.addAction(InputButton(InputButton::Device::GamepadButton, 4), prevScene);

	Generator::registerConstructor("CircleSpectrum", [&](){return new Gen_CircleSpectrum(&AC); });
	Generator::registerConstructor("Julia", [&](){return new Gen_Julia(&AC); });
	Generator::registerConstructor("Particles", [&](){return new Gen_Particles(&AC); });
	Generator::registerConstructor("Spectrum", [&](){return new Gen_Spectrum(&AC); });
	Generator::registerConstructor("Swarm", [&](){return new Gen_Swarm(&AC); });
	Generator::registerConstructor("Spirograph", [&](){return new Gen_Spirograph(&AC); });
	Generator::registerConstructor("Waveform", [&](){return new Gen_Waveform(&AC); });

	addScene("Julia");
	addScene("Spirograph");
	addScene("Particles");
	addScene("Swarm");
	addScene("Waveform");
	addScene("Spectrum");
	addScene("CircleSpectrum");

	UITexture.create(UIWidth * 2, windowHeight);
	panel = std::make_unique<UIPanel>(0, 0, UIWidth, windowHeight, &shaderList, scenes[0].get(), &palette,&UITexture);

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
	if (shaderList.size() == 0)
	{
		renderTexture[0].draw(sprite);
		window.draw(sf::Sprite(renderTexture[0].getTexture()));
		lastFrame.draw(sf::Sprite(renderTexture[0].getTexture()));
		lastFrame.display();
		renderTexture[0].clear();
		return;
	}
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

	sprite.setScale(1, -1);
	sprite.setPosition(0, windowHeight);//dump on gpu

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
	inputMap.clear();
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
		panel = std::make_unique<UIPanel>(0, 0, UIWidth, windowHeight, &shaderList, activeScene, &palette, &UITexture);

	}

	panel->doRefresh();
	subPanel.reset(nullptr);//hide panel on scene change
}

void App::requestParameterActionPanel(Parameter* param)
{
	subPanel = std::make_unique<ParameterActionPanel>(UIWidth, windowHeight, param, &inputMap, &UITexture);
}

void App::requestComboBoxPanel(UIComboBox* box)
{
	subPanel = std::make_unique<ComboBoxPanel>(UIWidth, windowHeight, box, &UITexture);
}

void App::serializeParameterList(std::ofstream& file, ParameterList list)
{
	file << list.size() << std::endl;
	for (auto &p : list)
	{
		file << p->getName() << std::endl;
		file << p->getValue() << std::endl;
		serializeLinkList(file, p);
	}
}

void App::deserializeLinkList(std::ifstream& file, Parameter* param)
{
	int noLinks = 0;
	file >> noLinks;
	for (int e = 0; e < noLinks; e++)
	{
		int device, button;
		file >> device;
		file >> button;
		InputButton inputButton((InputButton::Device)device, button);
		int type;
		float amount;
		file >> type;
		file >> amount;
		Action act(param, (Action::Type)type, amount);
		inputMap.addAction(inputButton, act);
	}
}

void App::serializeLinkList(std::ofstream& file, Parameter* p)
{
	auto links = inputMap.findParameterActions(p);
	file << std::to_string(links.size()) << std::endl;
	for (auto &l : links)
	{
		file << std::to_string((int)l.first->device) << std::endl;
		file << std::to_string((int)l.first->button) << std::endl;
		file << std::to_string((int)l.second->type) << std::endl;
		file << std::to_string(l.second->getAmount()) << std::endl;
	}
}

void App::save(std::string fname)
{
	std::ofstream file(fname, std::ios::trunc);
	std::cout << "Saving to file " << fname << "...";
	if (!file.is_open())
	{
		std::cout << "Could not open file\n";
		return;
	}

	file << scenes.size() << std::endl;
	
	for (auto &sc : scenes)
	{
		file << sc->getGenerator()->getName() << std::endl;
		serializeParameterList(file, sc->getGenerator()->getParameterList());
	}

	int sceneID = (int)getParameter("scene")->getValue();
	file << sceneID << std::endl;
	serializeLinkList(file, getParameter("scene"));

	file << shaderList.size() << std::endl;
	
	for (int c = 0; c < shaderList.size(); c++)
	{
		auto sh = shaderList.getShader(c);
		file << sh->getName() << std::endl;
		file << sh->isActive() << std::endl;
		serializeParameterList(file, sh->getParameterList());
	}

	serializeParameterList(file, palette.getParameterList());
	std::cout << "done!\n";
}

void App::load(std::string fname)
{
	std::ifstream file(fname);
	std::cout << "Loading file " << fname << "...";
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
			auto param = sc->getParameter(name);
			param->setValue(val);//try

			deserializeLinkList(file, param);
		}
	}

	file >> sceneID;
	lastSceneID = -1;//force scene switch
	getParameter("scene")->setValue(sceneID);
	deserializeLinkList(file, getParameter("scene"));

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
			auto param = sh->getParameter(name);
			param->setValue(val);//try

			deserializeLinkList(file, param);
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
		auto param = palette.getParameter(name);
		param->setValue(val);//try

		deserializeLinkList(file, param);
	}
	std::cout << "done!\n";
}

Scene* App::addScene(std::string sceneType)
{
	std::unique_ptr<Scene> scene = std::make_unique<Scene>(&AC, canvas.get());
	scene->setGenerator(Generator::construct(sceneType));
	scenes.push_back(std::move(scene));
	return scenes.back().get();
}