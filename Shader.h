#pragma once
#include "InputReciever.h"
#include <memory>
#include <fstream>

class Shader :
	public InputReciever
{
	sf::Shader* shader;
	std::string filename;
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
};

