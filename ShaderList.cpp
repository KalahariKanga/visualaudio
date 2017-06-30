#include "ShaderList.h"


ShaderList::ShaderList()
{
}


ShaderList::~ShaderList()
{
}

void ShaderList::addShader(std::string filename)
{
	shaders.push_back(std::make_unique<Shader>(filename));
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
		[sh](std::unique_ptr<Shader>& elem){return (elem.get() == sh); });
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
		[sh](std::unique_ptr<Shader>& elem){return (elem.get() == sh); });
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
		return shaders[pos].get();
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

std::vector<std::string> ShaderList::getShaderList()
{
	std::vector<std::string> list;
	list = {
		"shaders/bend",
		"shaders/blend",
		"shaders/bloom",
		"shaders/edgedetect",
		"shaders/kaleidoscope",
		"shaders/mosaic",
		"shaders/polar",
		"shaders/separatechannels",
		"shaders/tile"
	};
	return list;
}