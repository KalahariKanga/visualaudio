#pragma once

#include "Scene.h"

class App
{
	sf::RenderWindow window;
	sf::Sprite sprite;
	sf::Texture texture, finalTexture;
	sf::Image image;
	sf::RenderTexture lastFrame;
	sf::Clock clock;
	AudioCapture AC;

	int windowWidth = 800, windowHeight = 600;
	const int fps = 30;

	std::unique_ptr<Canvas> canvas;

	std::unique_ptr<Scene> scene;

	sf::Shader shader, blendShader;

public:
	App();
	~App();
	void update();
	bool quit = 0;

};

