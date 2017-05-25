#include "ColourView.h"
#include "UISlider.h"

using namespace UIStyle::Layout;

ColourView::ColourView(int x, int y, int w, int h, Parameter* _r, Parameter* _g, Parameter* _b)
	: UIElement(x, y, w, h), r(_r), g(_g), b(_b)
{
	addChild<UISlider>(x + hPad,				y, w / 3 - 2 * hPad, 12, r);//magic
	addChild<UISlider>(x + w/3 + hPad,			y, w / 3 - 2 * hPad, 12, g);
	addChild<UISlider>(x + 2 * w / 3 + hPad,	y, w / 3 - 2 * hPad, 12, b);
}


ColourView::~ColourView()
{
}

void ColourView::update()
{

}

void ColourView::refresh()
{

}