#include "ShaderList.h"
#include "ShaderStore.h"

ShaderStore ShaderList::shaderStore;

ShaderList::ShaderList()
{
	
}


ShaderList::~ShaderList()
{
}

void ShaderList::addShader(std::string name)
{
	try
	{
		shaders.push_back(shaderStore.getShader(name));
	}
	catch (...)
	{
		std::cout << "Could not find shader " << name << "\n";
	}
}

void ShaderList::removeShader(int pos)
{
	if (pos < 0 || pos >= size())
		return;
	shaders.erase(shaders.begin() + pos);
}

void ShaderList::removeShader(Shader* sh)
{
	auto it = std::find_if(shaders.begin(), shaders.end(),
		[sh](Shader& elem){return (&elem == sh); });
	if (it != shaders.end())
		shaders.erase(it);
}

void ShaderList::moveShader(int pos, int delta)
{
	if (pos < 0 || pos + delta < 0 || pos >= size() || pos + delta >= size())
		return;
	std::iter_swap(shaders.begin() + pos, shaders.begin() + pos + delta);
}

void ShaderList::moveShader(Shader* sh, int delta)
{
	auto it = std::find_if(shaders.begin(), shaders.end(),
		[sh](Shader& elem){return (&elem == sh); });
	if (it != shaders.end())
	{
		int index = it - shaders.begin();
		try
		{
			std::swap(shaders.at(index), shaders.at(index + delta));
		}
		catch (...)
		{
			//really though
		}
	}
}

Shader* ShaderList::getShader(int pos)
{
	try
	{
		return &shaders.at(pos);
	}
	catch (...)
	{
		return nullptr;
	}
}

int ShaderList::size()
{
	return shaders.size();
}
void ShaderList::clear()
{
	shaders.clear();
}

void ShaderList::loadShaders()
{
	shaderStore.loadShaders();
}