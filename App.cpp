#include "App.h"


App::App()
{
	window.create(sf::VideoMode(windowWidth, windowHeight), "Window");
	lastFrame.create(windowWidth, windowHeight);//!!!!
	lastFrame.clear();

	canvas = std::make_unique<Canvas>(windowWidth, windowHeight);
	scene = std::make_unique<Scene>(&AC);

	blendShader.loadFromFile("shaders/blend", sf::Shader::Fragment);
	blendShader.setParameter("lastFrame", lastFrame.getTexture());
	
	shader.loadFromFile("shaders/bloom", sf::Shader::Fragment);
	shader.setParameter("size_f", 4);
	//shader.setParameter("viewAngle", PI / 5);

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

	if (1)
		canvas->clear(sf::Color(0, 0, 0, 0));
	else
		canvas->wipe();//slow

	scene->update(*canvas);
	image.create(windowWidth, windowHeight, canvas->data);
	texture.loadFromImage(image);
	sprite.setTexture(texture);
	window.setView(sf::View(sf::FloatRect(0, 0, windowWidth, windowHeight)));

	if (1)
	{
		blendShader.setParameter("alpha", canvas->getParameter("clearAlpha").getValue());
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
			scene->addEvent(InputEvent(InputButton(InputButton::Device::Keyboard, (int)ev.key.code)));

		}
	}

	while (clock.getElapsedTime().asSeconds() < 1.0 / fps)
		std::this_thread::sleep_for(std::chrono::milliseconds(10));


}
