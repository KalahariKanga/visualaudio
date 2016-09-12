#pragma once
#include <thread>
#include <memory>
#include <unordered_map>
#include "Gen_Swarm.h"
#include "Gen_CircleSpectrum.h"
#include "Gen_Waveform.h"
#include "Gen_Spectrum.h"
#include "Gen_Particles.h"
#include "Palette.h"
#include "EventHandler.h"

class Scene :
	public InputReciever
{
	Canvas* canvas;
	std::unique_ptr<Generator> gen;
	EventHandler eventHandler;
	AudioCapture* ac;
public:
	Scene(AudioCapture* ac, Canvas* canvas);
	~Scene();
	void update();
	void addEvent(InputButton::Device device, int button, float data = 1);
	void addAction(InputButton input, Action action);
	template <class T> void setGenerator();

	Parameter* getParameter(std::string name) override;

};

template <class T>
void Scene::setGenerator()
{
	gen = std::make_unique<T>(ac);
}