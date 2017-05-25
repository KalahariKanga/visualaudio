#pragma once
#include "UIElement.h"
#include "Palette.h"

class PaletteView :
	public UIElement
{
	Palette* p;
	sf::Text title;
public:
	PaletteView(int x, int y, int w, int h, Palette* pal);
	~PaletteView();

	virtual void update() override;
	virtual void refresh() override;
	virtual void triggerCollapse() override;
};

