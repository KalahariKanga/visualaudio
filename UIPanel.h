#pragma once
#include "UIElement.h"
#include "ShaderListView.h"
#include "GeneratorView.h"
class UIPanel :
	public UIElement
{
	ShaderList* shaders;
	Scene* scene;
	Palette* palette;
public:
	UIPanel(int x, int y, int w, int h, ShaderList* shaders, Scene* sc, Palette* pal, sf::RenderTexture* texture);
	~UIPanel();
	virtual void update();
	virtual void refresh();
	virtual void processEvent(sf::Event ev);
	void rebuildChildren();
};

