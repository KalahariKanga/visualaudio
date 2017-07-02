#include "UIPanel.h"
#include "PaletteView.h"
#include "UIComboBox.h"

using namespace UIStyle::Layout;

UIPanel::UIPanel(int x, int y, int w, int h, ShaderList* shaders_, Scene* sc, Palette* pal, sf::RenderTexture* texture) : UIElement(x, y, w, h), palette(pal), scene(sc)
{
	this->shaders = shaders_;
	this->texture = texture;
	rebuildChildren();
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

void UIPanel::rebuildChildren()
{
	children.clear();

	addChild<PaletteView>(x, y, w, 16, palette);
	addChild<GeneratorView>(x, y, w, 16, scene);
	addChild<ShaderListView>(x, y, w, 16, shaders);
	std::function<void(std::string)> cb = [this](std::string str){shaders->addShader(str); rebuildChildren(); };
	addChild<UIComboBox>(x, y, w - 2 * hPad, 16, ShaderList::getShaderList(), cb);
	repositionChildren();
	requestRefresh();
}