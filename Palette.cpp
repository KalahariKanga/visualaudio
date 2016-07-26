#include "Palette.h"


Palette::Palette()
{
	addParameter("paletteSpeed", 0.002, 0, 0.1, 0.001);
	l0 = sf::Color::Red;
	h0 = sf::Color::Green;
	l1 = sf::Color::Blue;
	h1 = sf::Color::Yellow;
	pos = 0;
	speed = 0.002;
	build();
}


Palette::~Palette()
{
}

void Palette::build()
{
	for (int c = 0; c < 256; c++)
	{
		float y = (float)c / 256;
		sf::Color result;
		result.r = l0.r * (1 - pos) * (1 - y) + l1.r * pos * (1 - y) + h0.r * (1 - pos) * y + h1.r * pos * y;
		result.g = l0.g * (1 - pos) * (1 - y) + l1.g * pos * (1 - y) + h0.g * (1 - pos) * y + h1.g * pos * y;
		result.b = l0.b * (1 - pos) * (1 - y) + l1.b * pos * (1 - y) + h0.b * (1 - pos) * y + h1.b * pos * y;
		/*result.r = Math::clamp(result.r, 0, 255);
		result.g = Math::clamp(result.g, 0, 255);
		result.b = Math::clamp(result.b, 0, 255);*/
		colours[c] = result;
	}
}

void Palette::update()
{
	speed = getParameter("paletteSpeed").getValue();
	pos += speed * dir;
	if (pos > 1 || pos < 0)
		dir *= -1;
	build();
}

sf::Color Palette::getColour(float a)
{
	a = Math::clamp(a, 0, 1);
	return colours[a * 255];
}