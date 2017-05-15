#include "UIPanel.h"


UIPanel::UIPanel(int x, int y, int w, int h, ShaderList* shaders, Generator* gen, sf::RenderTexture* texture) : UIElement(x, y, w, h)
{
	this->shaders = shaders;
	this->generator = gen;
	this->texture = texture;
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