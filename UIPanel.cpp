#include "UIPanel.h"
#include "PaletteView.h"

UIPanel::UIPanel(int x, int y, int w, int h, ShaderList* shaders, Generator* gen, Palette* pal, sf::RenderTexture* texture) : UIElement(x, y, w, h)
{
	this->shaders = shaders;
	this->generator = gen;
	this->texture = texture;
	addChild<PaletteView>(x, y, w, 16, pal);
	addChild<GeneratorView>(x, y, w, 16, generator); 
	addChild<ShaderListView>(x, y, w, 16, shaders);
}


UIPanel::~UIPanel()
{
}

void UIPanel::update()
{

}

void UIPanel::refresh()
{
	if (!shaders)
		return;//look at how shaders is initialized

	repositionChildren();
}

void UIPanel::processEvent(sf::Event ev)
{
	if (ev.type == sf::Event::MouseWheelMoved)
	{
		if (ev.mouseWheel.x > x && ev.mouseWheel.x < x + w)
		{
			if (y + getH() > texture->getSize().y && ev.mouseWheel.delta < 0)
			{
				offsetPosition(0, ev.mouseWheel.delta * 16); //magic
				needRefresh = 1;
			}
			if (ev.mouseWheel.delta > 0 && y < 0)
			{
				offsetPosition(0, ev.mouseWheel.delta * 16);
				needRefresh = 1;
			}
		}
	}
}