#include "Shader.h"
#include <sstream>

const std::string Shader::preamble =	"uniform float aspectRatio;\n\
										uniform sampler2D texture;\n\
										uniform sampler2D lastFrame;\n";

Shader::Shader(std::string fname)
{
	shader = new sf::Shader();
	filename = fname;
	load(fname);
}

Shader& Shader::operator=(Shader&& other)
{
	delete shader;
	shader = other.shader;
	other.shader = nullptr;
	return *this;
}

Shader::Shader(Shader && other)
{
	delete shader;
	shader = other.shader;
	other.shader = nullptr;
}

Shader::~Shader()
{
	delete shader;
}

void Shader::load(std::string filename)
{
	std::ifstream file(filename);
	//if (!file.is_open())	:/
		
	std::stringstream buffer;
	buffer << file.rdbuf();
	auto shader_str = preamble + buffer.str();
	shader->loadFromMemory(shader_str, sf::Shader::Type::Fragment);

	parameters.clear();
	std::fstream params(filename + ".params");
	std::string name;
	char type;
	float def, min, max;
	if (params.is_open())
	{
		while (!params.eof())
		{
			try
			{
				params >> name >> type;
				switch (type)
				{
				case 'c':
					params >> def >> min >> max;
					addParameter(name, def, min, max);
					break;
				case 'd':
					params >> def >> min >> max;
					addParameter(name, def, min, max, Parameter::Type::Discrete);
					break;
				case 's':
					params >> def;
					addParameter(name, def, 0, 1, Parameter::Type::Switch);
					break;
				case 't':
					addParameter(name, 0, 0, 0, Parameter::Type::Trigger);
					break;
				default:
					throw std::exception();
				}
			}
			catch (...)
			{
				std::cout << "Error reading .params file for shader " << filename << std::endl;
				return;
			}
			
		}
	}
	else
		std::cout << "Could not find " << filename << ".params\n";
}

void Shader::update()
{
	for (auto p : parameters)
	{
		shader->setUniform(p.first, p.second.getValue());
	}
}