#pragma once
#include "UIElement.h"
#include "ShaderView.h"
#include "GeneratorView.h"
class UIPanel :
	public UIElement
{
	std::vector<std::unique_ptr<Shader>>* shaders;
public:
	UIPanel(int x, int y, int w, int h, std::vector<std::unique_ptr<Shader>>* shaders, Generator* gen);
	~UIPanel();
	virtual void update();
	virtual void refresh();
	Generator* generator;
};

