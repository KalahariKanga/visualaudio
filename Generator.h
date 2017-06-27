#pragma once
#include <vector>
#include <algorithm>
#include <functional>
#include "InputReciever.h"
#include "Canvas.h"
#include "AudioCapture.h"

class Generator : public InputReciever
{
	static std::map<std::string, std::function<Generator*(void)>> factoryMap;
protected:
	AudioCapture* const ac;
public:
	Generator(AudioCapture* AC);
	~Generator();
	virtual void update(Canvas& target, float deltaTime);//draw
	virtual std::string getName() = 0;
	static void registerConstructor(std::string generatorName, std::function<Generator*(void)> ctor);
	static Generator* construct(std::string generatorName);
};

