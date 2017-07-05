#pragma once

#include "Scene.h"
#include "RtMidi.h"
#include "Shader.h"
#include "UIPanel.h"
#include "ShaderList.h"
#include "ComboBoxPanel.h"
#include "ParameterActionWindow.h"
#include "SceneList.h"

class App : public InputReciever
{
	using ParameterList = std::vector < Parameter* > ;

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

	std::unique_ptr<Canvas> canvas;
	Palette palette;
	std::unique_ptr<SceneList> sceneList;

	std::unique_ptr<RtMidiIn> midiIn;

	InputMap inputMap;
	EventHandler eventHandler;
	
	ShaderList shaderList;

	std::unique_ptr<UIPanel> panel;
	std::unique_ptr<UISubPanel> subPanel;
	sf::RenderTexture UITexture, subUITexture;

	void setupMidi();

	void processEvents();
	void applyShaders();
	void debugShaders();
	void toggleFullscreen();
	void resize(int width, int height);
	void initialize();
	void rebuildUI();

	void serializeParameterList(std::ofstream& file, ParameterList list);
	void deserializeLinkList(std::ifstream& file, Parameter* param);
	void serializeLinkList(std::ofstream& file, Parameter* param);

public:
	App();
	~App();
	void update();
	bool quit = 0;

	void save(std::string fname);
	void load(std::string fname);

	void requestParameterActionPanel(Parameter* param);//pass down to parameterview somehow
	void requestComboBoxPanel(UIComboBox* box);
};

