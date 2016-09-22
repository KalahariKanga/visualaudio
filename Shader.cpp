#include "Shader.h"


Shader::Shader(std::string filename)
{
	shader = new sf::Shader();
	load(filename);
}

Shader& Shader::operator=(Shader&& other)
{
	shader = other.shader;
	other.shader = nullptr;
	return *this;
}

Shader::Shader(Shader && other)
{
	shader = other.shader;
	other.shader = nullptr;
}



Shader::~Shader()
{
	delete shader;
}

void Shader::load(std::string filename)
{
	shader->loadFromFile(filename, sf::Shader::Type::Fragment);
}