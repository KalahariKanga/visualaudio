#pragma once
#include <thread>
#include <memory>
#include <unordered_map>
#include "Gen_Swarm.h"
#include "Gen_CircleWaveform.h"
#include "Gen_CircleSpectrum.h"
#include "Gen_Waveform.h"
#include "Gen_Spectrum.h"
#include "Gen_Particles.h"
#include "Gen_Spirograph.h"
#include "Gen_Julia.h"
#include "Palette.h"
#include "EventHandler.h"

class Scene :
	public InputReciever
{
	Canvas* canvas;
	std::unique_ptr<Generator> gen;
	AudioCapture* ac;
public:
	Scene(AudioCapture* ac, Canvas* canvas);
	~Scene();
	void update(float deltaTime);

	void setGenerator(Generator* gen);
	Parameter* getParameter(std::string name) override;
	std::vector<Parameter*> getParameterList() override;
	Generator* getGenerator() { return gen.get(); } ///nonono
	void setParameterLock(bool lock);
};