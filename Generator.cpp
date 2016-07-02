#include "Generator.h"


Generator::Generator(AudioCapture* AC) : ac(AC)
{
}


Generator::~Generator()
{
}

void Generator::update(Canvas& target)
{
	for (int c = 0; c < 640; c++)
	{
		target.setDrawColour(sf::Color(c, 255 - c, 200));
		target.drawLine(c, 240, c, 240 + ac->getWaveform((float)c/640));
	}
}