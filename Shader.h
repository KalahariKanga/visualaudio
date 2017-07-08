#pragma once
#include "InputReciever.h"
#include <memory>
#include <fstream>

class Shader :
	public InputReciever
{
	sf::Shader* shaderProgram;
	std::string filename;
	bool active = 1;
	void loadParams();
public:
	Shader(std::string name, sf::Shader* shaderProgram);

	std::string getName() { return filename; }
	void update();
	void setActive(bool ac) { active = ac; }
	bool isActive() { return active; }
	void setUniform(std::string name, float value);
	void setUniform(std::string name, const sf::Texture& texture);
	void apply(const sf::Drawable& source, sf::RenderTarget& destination);
};

