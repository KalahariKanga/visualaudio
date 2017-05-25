#pragma once
#include "UIElement.h"
class ColourView :
	public UIElement
{
	Parameter *r, *g, *b;
public:
	ColourView(int x, int y, int w, int h, Parameter* r, Parameter* g, Parameter* b);
	~ColourView();

	virtual void update() override;
	virtual void refresh() override;
};

