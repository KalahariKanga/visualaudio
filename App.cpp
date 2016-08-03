#include "App.h"


App::App()
{
	window.create(sf::VideoMode(windowWidth, windowHeight), "Window");
	canvas = std::make_unique<Canvas>(windowWidth, windowHeight);
	gen = std::make_unique<Gen_Waveform>(&AC);

	Action paletteFaster(&canvas->getPalette().getParameter("paletteSpeed"), Action::Type::shift, 0.001);
	Action paletteSlower(&canvas->getPalette().getParameter("paletteSpeed"), Action::Type::shift, -0.001);
	Action fill(&gen->getParameter("fill"), Action::Type::trigger);
	Action alphaUp(&canvas->getParameter("clearAlpha"), Action::Type::shift, 0.1);
	Action alphaDown(&canvas->getParameter("clearAlpha"), Action::Type::shift, -0.1);
	keyboard.addAction((int)sf::Keyboard::Up, paletteFaster);
	keyboard.addAction((int)sf::Keyboard::Down, paletteSlower);
	keyboard.addAction((int)sf::Keyboard::Space, fill);
	keyboard.addAction((int)sf::Keyboard::Left, alphaDown);
	keyboard.addAction((int)sf::Keyboard::Right, alphaUp);

	shader.loadFromFile("shaders/shader", sf::Shader::Fragment);
	shader.setParameter("size_f", 4);
}


App::~App()
{
}

void App::update()
{
	sf::Event ev;
	clock.restart();
	AC.update();
	canvas->update();
	gen->update(*canvas);
	image.create(windowWidth, windowHeight, canvas->data);
	texture.loadFromImage(image);
	sprite.setTexture(texture);
	window.setView(sf::View(sf::FloatRect(0, 0, windowWidth, windowHeight)));
	window.draw(sprite,&shader);
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
			keyboard.addEvent((int)ev.key.code);

		}
	}

	keyboard.update();

	if (clock.getElapsedTime().asSeconds() < 1.0 / fps)
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
}