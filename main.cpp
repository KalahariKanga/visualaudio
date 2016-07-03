
#include "Generator.h"




int main(int argc, char** argv[])
{
	sf::RenderWindow window;
	window.create(sf::VideoMode(640, 480), "Window");
	sf::Sprite sprite;
	sf::Texture texture;
	sf::Image image;
	image.create(640, 480);

	AudioCapture AC;
	Generator gen(&AC);
	Canvas canvas(640, 480);
	sf::Event ev;
	bool quit = 0;
	while (!quit){ 
		AC.update(); 
		gen.update(canvas);
		image.create(640, 480, canvas.data);
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		window.draw(sprite);
		window.display();
		canvas.setDrawColour(sf::Color(0,0,0,32));
		canvas.drawRectangle(0, 0, 640, 480, 0);
		while (window.pollEvent(ev))
		{
			if (ev.type == sf::Event::Closed)
				quit = 1;
		}
	};
	return 0;
}