#pragma once
#include "InputReciever.h"
#include <memory>
#include <fstream>

class Shader :
	public InputReciever
{
	sf::Shader* shader;
	std::string filename;
	bool active = 1;
	static const std::string preamble;
public:
	Shader(std::string filename);
	Shader& operator=(Shader& other) = delete;
	Shader(Shader&) = delete;
	Shader& operator=(Shader&& other);
	Shader(Shader &&); //need move to emplace
	~Shader();
	void load(std::string filename);
	sf::Shader* getShader() { return shader; }
	std::string getName() { return filename; }
	void update();
	void setActive(bool ac) { active = ac; }
	bool isActive() { return active; }
};

