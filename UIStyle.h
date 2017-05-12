#pragma once
#include "SFML/graphics.hpp"
class UIStyle
{
public:
	struct Colour
	{
		static const sf::Color Primary;
		static const sf::Color Secondary;
		static const sf::Color PrimaryHighlight;
		static const sf::Color SecondaryHighlight;
		static const sf::Color PrimaryLowlight;
	};
	struct Text
	{
		static const int fontSize;
	};
};

