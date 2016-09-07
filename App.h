#pragma once

#include "Scene.h"

class App : public InputReciever
{
	sf::RenderWindow window;
	sf::Sprite sprite;
	sf::Texture texture;
	sf::Image image;
	sf::RenderTexture lastFrame;
	sf::Clock clock;
	AudioCapture AC;

	int windowWidth = 800, windowHeight = 600;
	const int fps = 60;

	std::unique_ptr<Canvas> canvas;

	std::vector<std::unique_ptr<Scene>> scenes;
	Scene* activeScene;

	sf::Shader shader, blendShader;

public:
	App();
	~App();
	void update();
	bool quit = 0;

	template <class T> Scene* addScene();


};

template <class T>
Scene* App::addScene()
{
	std::unique_ptr<Scene> scene = std::make_unique<Scene>(&AC, canvas.get());
	scene->setGenerator<T>();
	scenes.push_back(std::move(scene));
	return scenes.back().get();
}