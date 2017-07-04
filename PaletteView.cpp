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
	
	addChild<UIButton>(x + hPad, y + hPad, buttonSize, buttonSize, [&](){ triggerCollapse(); });

	addChild<ColourView>(x, y + 2 * hPad + hStep,		w, h, p->getParameter("r1"), p->getParameter("g1"), p->getParameter("b1"));
	addChild<ColourView>(x, y + 2 * hPad + 2 * hStep, w, h, p->getParameter("r2"), p->getParameter("g2"), p->getParameter("b2"));
	addChild<ColourView>(x, y + 2 * hPad + 3 * hStep, w, h, p->getParameter("r3"), p->getParameter("g3"), p->getParameter("b3"));
	addChild<ColourView>(x, y + 2 * hPad + 4 * hStep, w, h, p->getParameter("r4"), p->getParameter("g4"), p->getParameter("b4"));
	addChild<ParameterView>(x, y + 2 * hPad + 5 * hStep, w, h, p->getParameter("speed"));
}


PaletteView::~PaletteView()
{
}

void PaletteView::update()
{
	title.setPosition(x + 2 * hStep, y + 2);
	title.setString("Palette");
	draw(title);
}

void PaletteView::refresh()
{
	//repositionChildren();
}

void PaletteView::processEvent(sf::Event ev)
{
	/*if (ev.type == sf::Event::MouseMoved)
	{
		if (ev.mouseMove.y > y - 4 && ev.mouseMove.y < y + h + 4 && ev.mouseMove.x < w)
			children[0]->setActive(true);
		else
			children[0]->setActive(false);
	}*/
}

void PaletteView::triggerCollapse()
{
	collapsed = !collapsed;
	for (auto it = children.begin() + 1; it != children.end() - 1; it++)
		(*it)->triggerCollapse();
	children.back()->setActive(!collapsed);
	requestRefresh();
}