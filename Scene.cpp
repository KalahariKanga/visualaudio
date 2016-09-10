#include "Scene.h"


Scene::Scene(AudioCapture* _ac, Canvas* _canvas) : canvas(_canvas), ac(_ac)
{
	
}


Scene::~Scene()
{
}

void Scene::update()
{
	while (!eventList.empty())
	{
		InputEvent ev = eventList.front();
		eventList.pop_front();

		auto range = inputMap.equal_range(ev.button);
		for_each(range.first, range.second,
			[ev](InputMap::value_type& x)
			{
				auto action = x.second;
				action.execute(ev.data);
			}
		);
		
	}
	canvas->update();
	gen->update(*canvas);
}

void Scene::addEvent(InputEvent ev)
{
	eventList.push_back(ev);
}

void Scene::addEvent(InputButton::Device device, int button, float data)
{
	eventList.emplace_back(InputButton(device, button), data);
}

void Scene::addAction(InputButton input, Action action)
{
	inputMap.insert(InputMap::value_type(input, action));
}

Parameter* Scene::getParameter(std::string name)
{
	auto p = gen->getParameter(name);
	if (p) return p;
	p = canvas->getParameter(name);
	if (p) return p;
	return InputReciever::getParameter(name);
}