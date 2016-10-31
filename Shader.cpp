#include "Shader.h"


Shader::Shader(std::string fname)
{
	shader = new sf::Shader();
	filename = fname;
	load(fname);
}

Shader& Shader::operator=(Shader&& other)
{
	shader = other.shader;
	other.shader = nullptr;
	return *this;
}

Shader::Shader(Shader && other)
{
	shader = other.shader;
	other.shader = nullptr;
}

Shader::~Shader()
{
	delete shader;
}

void Shader::load(std::string filename)
{
	shader->loadFromFile(filename, sf::Shader::Type::Fragment);
	parameters.clear();
	std::fstream params(filename + ".params");
	std::string name;
	float def, min, max;
	if (params.is_open())
	{
		while (!params.eof())
		{
			try
			{
				params >> name >> def >> min >> max;
			}
			catch (...)
			{
				std::cout << "Error reading .params file for shader " << filename << std::endl;
			}
			addParameter(name, def, min, max);
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