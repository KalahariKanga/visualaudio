#include "Scene.h"


Scene::Scene(AudioCapture* ac)
{
	gen = std::make_unique<Gen_Waveform>(ac);
	Action fill(&gen->getParameter("fill"), Action::Type::trigger);
	Action position(&gen->getParameter("yPosition"), Action::Type::set, 1);
	addAction(InputButton(InputButton::Device::Keyboard, (int)sf::Keyboard::Space),fill);
	addAction(InputButton(InputButton::Device::GamepadButton, 1), fill);
	addAction(InputButton(InputButton::Device::GamepadAxis, 1), position);
}


Scene::~Scene()
{
}

void Scene::update(Canvas& target)
{
	while (!eventList.empty())
	{
		InputEvent ev = eventList.front();
		eventList.pop_front();

		auto range = inputMap.equal_range(ev.button);
		for_each(range.first, range.second,
			[ev](InputMap::value_type& x)
			{
				x.second.execute(ev.data);
			}
		);
	}
	gen->update(target);
}

void Scene::addEvent(InputEvent ev)
{
	eventList.push_back(ev);
}

void Scene::addAction(InputButton input, Action action)
{
	inputMap.insert(InputMap::value_type(input, action));
}