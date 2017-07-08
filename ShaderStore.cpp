#include "ShaderStore.h"
#include <sstream>

const std::string ShaderStore::preamble = "uniform float aspectRatio;\nuniform sampler2D texture;\nuniform sampler2D lastFrame;\n";

ShaderStore::ShaderStore()
{
	
}


ShaderStore::~ShaderStore()
{
}

void ShaderStore::loadShader(std::string filename)
{
	std::ifstream file("shaders/" + filename);
	if (!file.is_open())
	{
		std::cout << "Cannot open file shaders\\" << filename << "\n";
		return;
	}

	std::stringstream buffer;
	buffer << file.rdbuf();
	auto shader_str = preamble + buffer.str();
	shaderPrograms.push_back(std::unique_ptr<sf::Shader>(new sf::Shader()));
	std::cout << "Compiling " << filename << "...";
	bool compiled = shaderPrograms.back()->loadFromMemory(shader_str, sf::Shader::Type::Fragment);
	if (compiled)
		std::cout << "done\n";
	else
		std::cout << "falied\n";
	Shader shader(filename, shaderPrograms.back().get());
	shaders.insert(std::make_pair(filename, shader));
}

void ShaderStore::loadShaders()
{
	//had this in constructor, but it didn't work...:/
	shaderPrograms.clear();
	shaders.clear();
	auto list = getShaderList();
	for (auto &f : list)
		loadShader(f);
}

Shader ShaderStore::getShader(std::string name)
{
	return shaders.at(name);
}

std::vector<std::string> ShaderStore::getShaderList()
{
	std::vector<std::string> list;
	list = {
		"bend",
		"blend",
		"bloom",
		"edgedetect",
		"kaleidoscope",
		"mosaic",
		"polar",
		"separatechannels",
		"tile"
	};
	return list;
}