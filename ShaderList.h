#pragma once
#include "ShaderStore.h"
class ShaderList
{
	std::vector<Shader> shaders;
	static ShaderStore shaderStore;
public:
	ShaderList();
	~ShaderList();
	
	void addShader(std::string name);
	void removeShader(int pos);
	void removeShader(Shader* sh);
	void moveShader(int pos, int delta);
	void moveShader(Shader* sh, int delta);
	Shader* getShader(int pos);
	int size();
	void clear();
	static void loadShaders();
};

