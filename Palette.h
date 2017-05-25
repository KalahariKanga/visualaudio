#pragma once
#include "SFML/graphics.hpp"
#include <array>
#include "Math.h"
#include "InputReciever.h"

class Palette : public InputReciever
{
	float pos, speed;
	short dir = 1;
	void build();
	std::array<sf::Color, 256> colours;
public:
	Palette();
	~Palette();
	void update();
	sf::Color getColour(float a);
};

