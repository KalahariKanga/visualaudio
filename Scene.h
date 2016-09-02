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
	std::unique_ptr<Generator> gen;
	using InputMap = std::multimap < InputButton, Action > ;
	InputMap inputMap;
	std::list<InputEvent> eventList;
public:
	Scene(AudioCapture* ac);
	~Scene();
	void update(Canvas& target);
	void addEvent(InputEvent ev);
	void addAction(InputButton input, Action action);
};

