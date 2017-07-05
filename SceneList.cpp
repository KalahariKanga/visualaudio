#include "SceneList.h"


SceneList::SceneList(AudioCapture* ac_, Canvas* canvas_, std::function<void(void)> cb_) : ac(ac_), canvas(canvas_), callback(cb_)
{
	addParameter("scene", 0, 0, 16);

	//lock all scene parameters
	for (auto &s : scenes)
	{
		s->setParameterLock(true);
	}
}


SceneList::~SceneList()
{
}

void SceneList::onSceneChange()
{
	//change from lastSceneID to current scene
	scenes[lastSceneID]->setParameterLock(true);
	if (getCurentScene())
	{
		getCurentScene()->setParameterLock(false);
	}
	callback();//rebuildUI;
}

void SceneList::update(float deltaTime)
{
	int sceneID = (int)getParameter("scene")->getValue();
	if (sceneID >= scenes.size())
	{
		sceneID = scenes.size() - 1;
		getParameter("scene")->setValue(sceneID);
	}

	if (sceneID != lastSceneID)
		onSceneChange();
	lastSceneID = sceneID;

	getCurentScene()->update(deltaTime);
}

Scene* SceneList::addScene(std::string sceneType)
{
	std::unique_ptr<Scene> scene = std::make_unique<Scene>(ac, canvas);
	scene->setGenerator(Generator::construct(sceneType));
	scenes.push_back(std::move(scene));
	return scenes.back().get();
}

void SceneList::removeScene(int id)
{
	if (id >= 0 && id < scenes.size())
		scenes.erase(scenes.begin() + id);
}

Scene* SceneList::getScene(int id)
{
	return scenes[id].get();//catch
}

Scene* SceneList::getCurentScene()
{
	int id = (int)getParameter("scene")->getValue();
	return getScene(id);
}

void SceneList::setScene(int id)
{
	getParameter("scene")->setValue(id);
}

void SceneList::clear()
{
	scenes.clear();
}

int SceneList::size()
{
	return scenes.size();
}