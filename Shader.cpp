#include "Shader.h"




Shader::Shader(std::string name, sf::Shader* shaderProgram_) : shaderProgram(shaderProgram_), filename(name)
{
	loadParams();
}

void Shader::loadParams()
{
	parameters.clear();
	addParameter("bypass", 0, 0, 1, Parameter::Type::Switch);
	std::fstream params("shaders/" + filename + ".params");
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
	if (getParameter("bypass")->getValue())
		active = false;
	else
		active = true;
	for (auto p : parameters)
	{
		//kinda strange...
		shaderProgram->setUniform(p.first, p.second.getValue());
	}
}