#pragma once
#include "InputReciever.h"
#include "Scene.h"
class SceneList :
	public InputReciever
{
	AudioCapture* ac;
	Canvas* canvas;
	std::vector<std::unique_ptr<Scene>> scenes;
	
	int lastSceneID = 0;
	std::function<void(void)> callback;
public:
	SceneList(AudioCapture* ac_, Canvas* canvas_, std::function<void(void)> sceneChangeCB);
	~SceneList();

	void update(float deltaTime);

	Scene* addScene(std::string name);
	void removeScene(int id);
	Scene* getScene(int id);
	Scene* getCurentScene();
	void onSceneChange();
	void setScene(int id);
	void nextScene();
	void prevScene();
	void clear();

	int size();
};

