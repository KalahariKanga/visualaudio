#pragma once
#include "Shader.h"

class ShaderStore
{
	std::vector<std::unique_ptr<sf::Shader>> shaderPrograms;
	std::map<std::string, Shader> shaders;
	void loadShader(std::string fname);
	static const std::string preamble;
public:
	ShaderStore();
	~ShaderStore();

	Shader getShader(std::string name);
	void loadShaders();
	static std::vector<std::string> getShaderList();
};

