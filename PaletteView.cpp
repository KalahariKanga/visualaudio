#include "PaletteView.h"
#include "ColourView.h"
#include "UIButton.h"
#include "ParameterView.h"

using namespace UIStyle::Layout;

PaletteView::PaletteView(int x, int y, int w, int h, Palette* pal) : UIElement(x, y, w, h), p(pal)
{
	title.setFont(*UIElement::getFont());
	title.setCharacterSize(UIStyle::Text::fontSize);
	title.setFillColor(UIStyle::Colour::Primary);
	
	addChild<UIButton>(hPad, y + hStep / 2, buttonSize, buttonSize, [&](){ triggerCollapse(); });
	
	addChild<ColourView>(x, y, w, h, p->getParameter("r1"), p->getParameter("g1"), p->getParameter("b1"));
	addChild<ColourView>(x, y, w, h, p->getParameter("r2"), p->getParameter("g2"), p->getParameter("b2"));
	addChild<ColourView>(x, y, w, h, p->getParameter("r3"), p->getParameter("g3"), p->getParameter("b3"));
	addChild<ColourView>(x, y, w, h, p->getParameter("r4"), p->getParameter("g4"), p->getParameter("b4"));
	addChild<ParameterView>(x, y, w, h, p->getParameter("speed"));
}


PaletteView::~PaletteView()
{
}

void PaletteView::update()
{
	title.setPosition(x + hStep, y + hPad);
	title.setString("Palette");
	draw(title);
}

void PaletteView::refresh()
{
	repositionChildren();
}

void PaletteView::triggerCollapse()
{
	collapsed = !collapsed;
	for (auto it = children.begin() + 1; it != children.end(); it++)
		(*it)->triggerCollapse();
	requestRefresh();
}