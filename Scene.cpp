#include "Scene.h"


Scene::Scene(AudioCapture* ac, Canvas* _canvas) : canvas(_canvas)
{
	gen = std::make_unique<Gen_Swarm>(ac);

	Action fill(gen->getParameter("fill"), Action::Type::trigger);
	Action position(gen->getParameter("yPosition"), Action::Type::set, 1);
	Action alpha(canvas->getParameter("clearAlpha"), Action::Type::set, 1);
	Action aiUp(gen->getParameter("ai"), Action::Type::shift, 1);
	Action aiDown(gen->getParameter("ai"), Action::Type::shift, -1);
	addAction(InputButton(InputButton::Device::Keyboard, (int)sf::Keyboard::Space),fill);
	addAction(InputButton(InputButton::Device::GamepadButton, 1), fill);
	addAction(InputButton(InputButton::Device::GamepadAxis, 1), position);
	addAction(InputButton(InputButton::Device::GamepadAxis, 2), alpha);
	addAction(InputButton(InputButton::Device::Keyboard, (int)sf::Keyboard::Up),aiUp);
	addAction(InputButton(InputButton::Device::Keyboard, (int)sf::Keyboard::Down),aiDown);
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