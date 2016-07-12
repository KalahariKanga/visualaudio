

#include <iostream>
#include <thread>
#include "Gen_Swarm.h"
#include "Palette.h"


int main(int argc, char** argv[])
{
	int windowWidth = 640, windowHeight = 480;
	const int fps = 30;
	sf::RenderWindow window;
	window.create(sf::VideoMode(windowWidth, windowHeight), "Window");
	sf::Sprite sprite;
	sf::Texture texture;
	sf::Image image;

	AudioCapture AC;
	Palette palette;
	Gen_Swarm gen(&AC);
	Canvas canvas(windowWidth, windowHeight);
	canvas.pal = &palette;
	sf::Event ev;
	bool quit = 0;
	sf::Clock clock;
	while (!quit){
		clock.restart();
		AC.update(); 
		gen.update(canvas);
		palette.update();
		image.create(windowWidth, windowHeight, canvas.data);
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		window.setView(sf::View(sf::FloatRect(0, 0, windowWidth, windowHeight)));
		window.draw(sprite);
		window.display();

		canvas.setDrawColour(sf::Color(0,0,0,255));
		canvas.drawRectangle(0, 0, windowWidth, windowHeight, 0);

		while (window.pollEvent(ev))
		{
			if (ev.type == sf::Event::Closed)
				quit = 1;
			if (ev.type == sf::Event::Resized)
			{
				windowWidth = ev.size.width;
				windowHeight = ev.size.height;
				canvas.resize(windowWidth, windowHeight);
				std::cout << windowWidth << " " << windowHeight << "\n";
			}
			if (ev.type == sf::Event::KeyPressed)
			{
				switch (ev.key.code)
				{
				case sf::Keyboard::N:
					AC.normalise();
					break;
				case sf::Keyboard::Up:
					gen.getParameter("noParts").increment();
					break;
				case sf::Keyboard::Down:
					gen.getParameter("noParts").decrement();
					break;
				}
			}
		}

		if (clock.getElapsedTime().asSeconds() < 1.0/fps)
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
	};
	return 0;
}