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
	void removeShader(Shader* sh);
	void moveShader(int pos, int delta);
	void moveShader(Shader* sh, int delta);
	Shader* getShader(int pos);
	int size();
	void clear();
};

