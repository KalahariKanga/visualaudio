#include "Generator.h"
#include <iostream>

std::map<std::string, std::function<Generator*(void)>> Generator::factoryMap;

Generator::Generator(AudioCapture* AC) : ac(AC)
{
}


Generator::~Generator()
{
}

void Generator::update(Canvas& target, float deltaTime)
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
		target.setDrawColour((float)c/width);
		target.drawLine(c, height, c, height - (height / 2)*ac->getFFT((float)c / width));
		target.drawLine(c, height / 2 - ac->getWaveform((float)c / width), c, height / 2 + ac->getWaveform((float)c / width));
	}
}

void Generator::registerConstructor(std::string generatorName, std::function<Generator*(void)> ctor)
{
	factoryMap[generatorName] = ctor;
}

Generator* Generator::construct(std::string generatorName)
{
	try
	{
		auto gen = factoryMap[generatorName]();
		gen->name = generatorName;
		return gen;
	}
	catch (...)
	{
		std::cout << "illegal generator name!!\n";
		return nullptr;
	}
}

std::vector<std::string> Generator::getGeneratorList()
{
	std::vector<std::string> list;
	for (auto &kv : factoryMap)
	{
		list.push_back(kv.first);
	}
	return list;
}