#pragma once
#include "SFML/graphics.hpp"
namespace UIStyle
{
	namespace Colour
	{
		static const sf::Color Primary(255, 255, 255);
		static const sf::Color Secondary(255, 0, 0);
		static const sf::Color PrimaryHighlight(255, 200, 200);
		static const sf::Color SecondaryHighlight(255, 255, 0);
		static const sf::Color PrimaryLowlight(92, 92, 92);
	};
	namespace Text
	{
		static const int fontSize = 12;
	};
	namespace Layout
	{
		static const int hPad = 4;
		static const int wPad = 4;
		static const int hStep = 16;
		static const int buttonSize = 12;
		static const int sliderH = 12;
	};
};

