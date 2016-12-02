#pragma once
#include "UIElement.h"
#include "ShaderListView.h"
#include "GeneratorView.h"
class UIPanel :
	public UIElement
{
	ShaderList* shaders;
public:
	UIPanel(int x, int y, int w, int h, ShaderList* shaders, Generator* gen);
	~UIPanel();
	virtual void update();
	virtual void refresh();
	Generator* generator;
};

