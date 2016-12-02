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

void ShaderList::moveShader(int pos, int delta)
{
	if (pos < 0 || pos + delta < 0 || pos >= size() || pos + delta >= size())
		return;
	std::iter_swap(shaders.begin() + pos, shaders.begin() + pos + delta);
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