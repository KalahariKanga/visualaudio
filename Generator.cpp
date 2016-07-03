#include "Generator.h"


Generator::Generator(AudioCapture* AC) : ac(AC)
{
}


Generator::~Generator()
{
}

void Generator::update(Canvas& target)
{
	/*for (int c = 0; c < 640; c++)
	{
		target.setDrawColour(sf::Color(c/3, 255*((float)c/640), 200));
		target.drawLine(c, 240, c, 240 + ac->getWaveform((float)c/640));
	}*/
	target.setDrawColour(sf::Color(0, 255, 100));
	target.drawRectangle(0, 0, 640, ac->getAmplitude(), 0);
}

void Generator::addParameter(std::string name, float value, float min, float max)
{
	parameters.emplace_back(name, value, min, max);
}