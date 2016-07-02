
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
	while (1){ 
		AC.update(); 
		gen.update(canvas);
		image.create(640, 480, canvas.data);
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		window.draw(sprite);
		window.display();
		canvas.clear(sf::Color(0, 0, 0));
	};
	return 0;
}