#pragma once
#include "UIElement.h"
#include "SceneList.h"

class SceneListView :
	public UIElement
{
	SceneList* list;
public:
	SceneListView(int x, int y, int w, int h, SceneList* list_);
	~SceneListView();

	virtual void update() override;
};

