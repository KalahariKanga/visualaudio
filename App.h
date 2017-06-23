#pragma once

#include "Scene.h"
#include "RtMidi.h"
#include "Shader.h"
#include "UIPanel.h"
#include "ShaderList.h"
#include "ParameterActionWindow.h"

class App : public InputReciever
{
	sf::RenderWindow window;
	sf::Sprite sprite;
	sf::Texture texture;
	sf::Image image;
	sf::RenderTexture renderTexture[2], lastFrame;
	sf::Clock clock;
	AudioCapture AC;

	int windowWidth = 800, windowHeight = 600;
	int UIWidth = 192;
	bool fullscreen = 0;
	bool showUI = 0;
	const int fps = 60;
	float lastFrameTime = 1 / 60;
	int sceneID = 0, lastSceneID = -1;

	std::unique_ptr<Canvas> canvas;
	Palette palette;
	std::vector<std::unique_ptr<Scene>> scenes;
	Scene* activeScene;

	std::unique_ptr<RtMidiIn> midiIn;

	InputMap inputMap;
	EventHandler eventHandler;
	
	ShaderList shaderList;

	std::unique_ptr<UIPanel> panel;
	std::unique_ptr<ParameterActionPanel> subPanel;
	sf::RenderTexture UITexture, subUITexture;

	void setupMidi();

	void processEvents();
	void applyShaders();
	void debugShaders();
	void toggleFullscreen();
	void resize(int width, int height);
	void initialize();
	void changeScene(int id);
public:
	App();
	~App();
	void update();
	bool quit = 0;

	void save(std::string fname);
	void load(std::string fname);

	template <class T> Scene* addScene();
	Scene* addScene(std::string sceneType);

	void requestParameterActionPanel(Parameter* param);//pass down to parameterview somehow
};

template <class T>
Scene* App::addScene()
{
	std::unique_ptr<Scene> scene = std::make_unique<Scene>(&AC, canvas.get());
	scene->setGenerator<T>();
	scenes.push_back(std::move(scene));
	return scenes.back().get();
}

