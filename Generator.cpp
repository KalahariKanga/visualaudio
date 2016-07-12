#include "Generator.h"
#include <iostream>

Generator::Generator(AudioCapture* AC) : ac(AC)
{
}


Generator::~Generator()
{
}

void Generator::update(Canvas& target)
{
	int width = target.getWidth();
	int height = target.getHeight();
	/*for (int c = 0; c < 640; c++)
	{
		target.setDrawColour(sf::Color(c/3, 255*((float)c/640), 200));
		target.drawLine(c, 240, c, 240 + ac->getWaveform((float)c/640));
	}*/
	
	for (int c = 0; c < width; c++)
	{
		target.setDrawColour(target.pal->getColour((float)c/width));
		target.drawLine(c, height, c, height - (height / 2)*ac->getFFT((float)c / width));
		target.drawLine(c, height / 2, c, height / 2 + ac->getWaveform((float)c / width));
	}
}

void Generator::addParameter(std::string name, float value, float min, float max)
{
	parameters.insert(std::map<std::string, Parameter>::value_type(name, Parameter(value, min, max)));
}

Parameter& Generator::getParameter(std::string name)
{
	try
	{
		return parameters.at(name);
	}
	catch (std::out_of_range)
	{
		std::cout << "Error: Unknown Parameter Name: " << name << "\n";
	}
}