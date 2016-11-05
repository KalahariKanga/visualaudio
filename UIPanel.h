#pragma once
#include "UIElement.h"
#include "ShaderView.h"
#include "GeneratorView.h"
class UIPanel :
	public UIElement
{
public:
	UIPanel(int x, int y, int w, int h);
	~UIPanel();
	virtual void update();
	std::vector<std::unique_ptr<Shader>>* shaders;//like no
	Generator* generator;//really no
	virtual void refresh();
};

