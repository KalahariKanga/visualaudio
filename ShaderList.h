#pragma once
#include "Shader.h"
class ShaderList
{
	std::vector<std::unique_ptr<Shader>> shaders;
public:
	ShaderList();
	~ShaderList();

	void addShader(std::string filename);
	void removeShader(int pos);
	void moveShader(int pos, int delta);
	Shader* getShader(int pos);
	int size();
};

