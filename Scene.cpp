#include "Scene.h"


Scene::Scene(AudioCapture* _ac, Canvas* _canvas) : canvas(_canvas), ac(_ac)
{
	
}


Scene::~Scene()
{
}

void Scene::update()
{
	eventHandler.update();
	canvas->update();
	gen->update(*canvas);
}

void Scene::addEvent(InputButton::Device device, int button, float data)
{
	eventHandler.addEvent(device, button, data);
}

void Scene::addAction(InputButton input, Action action)
{
	eventHandler.addAction(input, action);
}

Parameter* Scene::getParameter(std::string name)
{
	auto p = gen->getParameter(name);
	if (p) return p;
	p = canvas->getParameter(name);
	if (p) return p;
	return InputReciever::getParameter(name);
}