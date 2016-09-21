#pragma once
#include "InputReciever.h"
#include <memory>

class Shader :
	public InputReciever
{
	sf::Shader* shader;
public:
	Shader(std::string filename);
	Shader& operator=(Shader&& other);
	Shader(const Shader &&); //need move to emplace
	~Shader();
	void load(std::string filename);
	sf::Shader* getShader() { return shader; }
};

