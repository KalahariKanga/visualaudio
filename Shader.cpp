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