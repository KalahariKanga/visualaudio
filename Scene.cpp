#include "Scene.h"


Scene::Scene(AudioCapture* _ac, Canvas* _canvas) : canvas(_canvas), ac(_ac)
{
	
}


Scene::~Scene()
{
}

void Scene::update(float deltaTime)
{
	gen->update(*canvas, deltaTime);
}

Parameter* Scene::getParameter(std::string name)
{
	auto p = gen->getParameter(name);
	if (p) return p;
	return InputReciever::getParameter(name);
}

std::vector<Parameter*> Scene::getParameterList()
{
	auto list = InputReciever::getParameterList();
	auto genList = gen->getParameterList();
	std::copy(genList.begin(), genList.end(), std::back_inserter(list));
	return list;
}

void Scene::setParameterLock(bool lock)
{
	auto params = getParameterList();
	for (auto &p : params)
	{
		p->setLock(lock);
	}
}

void Scene::setGenerator(Generator* gen_)
{
	gen.reset(gen_);
}