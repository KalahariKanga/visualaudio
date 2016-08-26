#pragma once
#include "KeyboardInput.h"
#include "Gen_Swarm.h"
#include "Gen_CircleSpectrum.h"
#include "Gen_Waveform.h"
#include "Palette.h"
#include <thread>
#include <memory>

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
	std::unique_ptr<Generator> gen;

	KeyboardInput keyboard;

	sf::Shader shader, blendShader;

public:
	App();
	~App();
	void update();
	bool quit = 0;

};

