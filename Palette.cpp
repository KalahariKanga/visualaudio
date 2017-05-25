#include "Palette.h"


Palette::Palette()
{
	addParameter("speed", 0.002, 0, 0.1);

	addParameter("r1", 255, 0, 255);
	addParameter("g1", 0, 0, 255);
	addParameter("b1", 0, 0, 255);
	addParameter("r2", 0, 0, 255);
	addParameter("g2", 255, 0, 255);
	addParameter("b2", 0, 0, 255);
	addParameter("r3", 128, 0, 255);
	addParameter("g3", 128, 0, 255);
	addParameter("b3", 0, 0, 255);
	addParameter("r4", 255, 0, 255);
	addParameter("g4", 0, 0, 255);
	addParameter("b4", 255, 0, 255);
	
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
		sf::Color l0(getParameter("r1")->getValue(), getParameter("g1")->getValue(), getParameter("b1")->getValue());
		sf::Color h0(getParameter("r2")->getValue(), getParameter("g2")->getValue(), getParameter("b2")->getValue());
		sf::Color l1(getParameter("r3")->getValue(), getParameter("g3")->getValue(), getParameter("b3")->getValue());
		sf::Color h1(getParameter("r4")->getValue(), getParameter("g4")->getValue(), getParameter("b4")->getValue());
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
	speed = getParameter("speed")->getValue();
	pos += speed * dir;
	if (pos > 1)
	{
		pos = 1;
		dir = -1;
	}
	if (pos < 0)
	{
		pos = 0;
		dir = 1;
	}
	build();
}

sf::Color Palette::getColour(float a)
{
	a = Math::clamp(a, 0, 1);
	return colours[a * 255];
}