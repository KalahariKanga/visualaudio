#include "App.h"
#include "ParameterView.h"
#include <fstream>

std::function<void(Parameter*)> App::parameterActionPopup;
std::function<void(UIComboBox*)> App::comboBoxPopup;

App::App()
{
	renderTexture[0].create(windowWidth, windowHeight);
	renderTexture[1].create(windowWidth, windowHeight);
	lastFrame.create(windowWidth, windowHeight);

	canvas = std::make_unique<Canvas>(windowWidth, windowHeight, &palette);
	midiIn = std::make_unique<RtMidiIn>();
	sceneList = std::make_unique<SceneList>(&AC, canvas.get(), [&]{rebuildUI();} );
	setupMidi();
	//midiIn->openPort(3);

	ShaderList::loadShaders();
	
	window.create(sf::VideoMode(windowWidth, windowHeight), "Window");
	//window.setVerticalSyncEnabled(true);

	parameterActionPopup = [this](Parameter* p){ requestParameterActionPanel(p); };
	comboBoxPopup = [this](UIComboBox* box){ requestComboBoxPanel(box); };
	LinkView::nextButton = [this](){ return eventHandler.nextButton(); };

	eventHandler.setInputMap(&inputMap);

	Action nextScene(sceneList->getParameter("scene"), Action::Type::shift, 1);
	Action prevScene(sceneList->getParameter("scene"), Action::Type::shift, -1);

	inputMap.addAction(InputButton(InputButton::Device::Keyboard, (int)sf::Keyboard::Right), nextScene);
	inputMap.addAction(InputButton(InputButton::Device::Keyboard, (int)sf::Keyboard::Left), prevScene);

	inputMap.addAction(InputButton(InputButton::Device::GamepadButton, 5), nextScene);
	inputMap.addAction(InputButton(InputButton::Device::GamepadButton, 4), prevScene);

	Generator::registerConstructor("CircleSpectrum", [&](){return new Gen_CircleSpectrum(&AC); });
	Generator::registerConstructor("CircleWaveform", [&](){return new Gen_CircleWaveform(&AC); });
	Generator::registerConstructor("Julia", [&](){return new Gen_Julia(&AC); });
	Generator::registerConstructor("Particles", [&](){return new Gen_Particles(&AC); });
	Generator::registerConstructor("Spectrum", [&](){return new Gen_Spectrum(&AC); });
	Generator::registerConstructor("Swarm", [&](){return new Gen_Swarm(&AC); });
	Generator::registerConstructor("Spirograph", [&](){return new Gen_Spirograph(&AC); });
	Generator::registerConstructor("Waveform", [&](){return new Gen_Waveform(&AC); });

	sceneList->addScene("Waveform");

	UITexture.create(UIWidth * 2, windowHeight);
	panel = std::make_unique<UIPanel>(0, 0, UIWidth, windowHeight, sceneList.get(), &palette,&UITexture);
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
	
	AC.update();
	palette.update();
	canvas->clear(sf::Color(0, 0, 0, 0));
	sceneList->update(fps * lastFrameTime);//2 -> half fps -> move twice
	
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
	
	Parameter::untriggerAll();
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
			try
			{
				panel->distributeEvent(ev);
				if (subPanel.get())
					subPanel->distributeEvent(ev);
			}
			catch (SquashEventException sq)
			{

			}
		}
	}
	std::vector<unsigned char> message;
	while (midiIn->isPortOpen())
	{
		midiIn->getMessage(&message);
		if (message.empty())
			break;
		if (message[0] >= 144 && message[0] <= 159 && message[2] > 0)//10010000 to 10011111 - note on, >0 velocity
			eventHandler.addEvent(InputButton::Device::MIDINote, (int)message[1]);
		if (message[0] >= 176 && message[0] <= 191) //10110000 to 10111111 - control change
			eventHandler.addEvent(InputButton::Device::MIDICV, (int)message[1], (float)message[2] / 128);
	}
}

void App::applyShaders()
{
	auto shaderList = sceneList->getCurentScene()->getShaderList();
	//probably could be simpler
	if (shaderList->size() == 0)
	{
		renderTexture[0].draw(sprite);
		window.draw(sf::Sprite(renderTexture[0].getTexture()));
		lastFrame.draw(sf::Sprite(renderTexture[0].getTexture()));
		lastFrame.display();
		renderTexture[0].clear();
		return;
	}
	
	sprite.setScale(1, -1);
	sprite.setPosition(0, windowHeight);//dump on gpu

	shaderList->getShader(0)->update();
	shaderList->getShader(0)->setUniform("aspectRatio", (float)windowWidth / windowHeight);
	shaderList->getShader(0)->setUniform("lastFrame", lastFrame.getTexture());
	shaderList->getShader(0)->apply(sprite, renderTexture[0]);
	renderTexture[0].display();

	int t = 1;
	for (; t < shaderList->size(); t++)
	{
		shaderList->getShader(t)->update();
		shaderList->getShader(t)->setUniform("aspectRatio", (float)windowWidth / windowHeight);
		shaderList->getShader(t)->setUniform("lastFrame", lastFrame.getTexture());
		shaderList->getShader(t)->apply(sf::Sprite(renderTexture[(t + 1) % 2].getTexture()), renderTexture[t % 2]);
		renderTexture[t % 2].display();
	}

	window.draw(sf::Sprite(renderTexture[(t + 1) % 2].getTexture()));
	lastFrame.draw(sf::Sprite(renderTexture[(t + 1) % 2].getTexture()));
	lastFrame.display();
	renderTexture[0].clear();
}

//void App::debugShaders()
//{
//	//dumb as heck, dont actually use
//	auto shaderList = sceneList->getCurentScene()->getShaderList();
//
//	static sf::RenderTexture temptex;
//	static bool created = 0;
//	if (!created)
//	{
//		temptex.create(windowWidth, windowHeight);
//		created = 1;
//	}
//	if (shaderList->size() == 0) return;
//	shaderList->getShader(0)->update();
//	shaderList->getShader(0)->setUniform("aspectRatio", (float)windowWidth / windowHeight);//omg :(
//	shaderList->getShader(0)->setUniform("lastFrame", lastFrame.getTexture());
//
//	sprite.setScale(1, -1);
//	sprite.setPosition(0, windowHeight);//dump on gpu
//
//	if (shaderList->getShader(0)->isActive())
//	{
//		sf::Sprite spr;
//		spr.setTexture(renderTexture[0].getTexture());
//		spr.setScale(0.16, 0.16);
//		spr.setPosition(windowWidth*5.0/6.0, 0);
//		temptex.draw(spr, shaderList->getShader(0)->getShader());
//		renderTexture[0].draw(sprite, shaderList->getShader(0)->getShader());
//	}
//	else
//	{
//		sf::Sprite spr;
//		spr.setTexture(renderTexture[0].getTexture());
//		spr.setScale(0.16, 0.16);
//		spr.setPosition(windowWidth*5.0 / 6.0, 0);
//		temptex.draw(spr);
//		renderTexture[0].draw(sprite);
//	}
//
//	renderTexture[0].display();
//	int t = 1;
//	for (; t < shaderList->size(); t++)
//	{
//		shaderList->getShader(t)->update();
//		shaderList->getShader(t)->getShader()->setUniform("aspectRatio", (float)windowWidth / windowHeight);
//		shaderList->getShader(t)->getShader()->setUniform("lastFrame", lastFrame.getTexture());
//
//		if (shaderList->getShader(t)->isActive())
//		{
//			sf::Sprite spr;
//			spr.setTexture(renderTexture[(t + 1) % 2].getTexture());
//			spr.setScale(0.16, 0.16);
//			spr.setPosition(windowWidth*5.0 / 6.0, t*windowHeight / 6.0);
//			temptex.draw(spr, shaderList->getShader(t)->getShader());
//			renderTexture[t % 2].draw(sf::Sprite(renderTexture[(t + 1) % 2].getTexture()), shaderList->getShader(t)->getShader());
//		}
//		else
//		{
//			sf::Sprite spr;
//			spr.setTexture(renderTexture[(t + 1) % 2].getTexture());
//			spr.setScale(0.16, 0.16);
//			spr.setPosition(windowWidth*5.0 / 6.0, t*windowHeight / 6.0);
//			temptex.draw(spr);
//			renderTexture[t % 2].draw(sf::Sprite(renderTexture[(t + 1) % 2].getTexture()));
//		}
//		renderTexture[t % 2].display();
//	}
//	window.draw(sf::Sprite(renderTexture[(t + 1) % 2].getTexture()));
//	window.draw(sf::Sprite(temptex.getTexture()));
//	lastFrame.draw(sf::Sprite(renderTexture[(t + 1) % 2].getTexture()));
//	lastFrame.display();
//	temptex.display();
//	renderTexture[0].clear();
//}

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
	sceneList->clear();
	inputMap.clear();
	//eventHandler.clear();//impl
	//shaderList.clear();

	sceneList->setScene(0);//could do something here re: onscenechange being public
}

void App::rebuildUI()
{
	panel = std::make_unique<UIPanel>(0, 0, UIWidth, windowHeight, sceneList.get(), &palette, &UITexture);
	panel->doRefresh();
	subPanel.reset(nullptr);
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

	file << sceneList->size() << std::endl;
	
	for (int c = 0; c < sceneList->size(); c++)
	{
		auto sc = sceneList->getScene(c);
		file << sc->getGenerator()->getName() << std::endl;
		serializeParameterList(file, sc->getGenerator()->getParameterList());
		
		auto shl = sc->getShaderList();
		file << shl->size() << std::endl;
	
		for (int c = 0; c < shl->size(); c++)
		{
			auto sh = shl->getShader(c);
			file << sh->getName() << std::endl;
			file << sh->isActive() << std::endl;
			serializeParameterList(file, sh->getParameterList());
		}

	}

	int sceneID = (int)sceneList->getParameter("scene")->getValue();
	file << sceneID << std::endl;
	serializeLinkList(file, sceneList->getParameter("scene"));

	
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
		sc = sceneList->addScene(scenetype);
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
		int noShaders = 0;
		file >> noShaders;
		for (int c = 0; c < noShaders; c++)
		{
			std::string name;
			bool active;
			file >> name;
			file >> active;
			sc->getShaderList()->addShader(name);//try
			auto sh = sc->getShaderList()->getShader(c);
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
	}

	int scene;
	file >> scene;
	sceneList->getParameter("scene")->setValue(scene);
	deserializeLinkList(file, sceneList->getParameter("scene"));

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

	sceneList->onSceneChange();

	std::cout << "done!\n";
}