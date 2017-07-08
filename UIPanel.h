#pragma once
#include "UIElement.h"
#include "ShaderListView.h"
#include "GeneratorView.h"
#include "SceneListView.h"

class UIPanel :
	public UIElement
{
	SceneList* sceneList;
	Palette* palette;
public:
	UIPanel(int x, int y, int w, int h, SceneList* sl, Palette* pal, sf::RenderTexture* texture);
	~UIPanel();
	virtual void update();
	virtual void refresh();
	virtual void processEvent(sf::Event ev);
	void rebuildChildren();
};

