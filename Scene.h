#pragma once
#include <thread>
#include <memory>
#include <unordered_map>
#include "Action.h"
#include "Gen_Swarm.h"
#include "Gen_CircleSpectrum.h"
#include "Gen_Waveform.h"
#include "Gen_Spectrum.h"
#include "Palette.h"
#include "InputEvent.h"

class Scene :
	public InputReciever
{
	Canvas* canvas;
	std::unique_ptr<Generator> gen;
	using InputMap = std::multimap < InputButton, Action > ;
	InputMap inputMap;
	std::list<InputEvent> eventList;
	AudioCapture* ac;
public:
	Scene(AudioCapture* ac, Canvas* canvas);
	~Scene();
	void update();
	void addEvent(InputEvent ev);
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