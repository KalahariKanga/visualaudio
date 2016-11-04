#include "UIPanel.h"


UIPanel::UIPanel(int x, int y, int w, int h) : UIElement(x, y, w, h)
{
	refresh();
}


UIPanel::~UIPanel()
{
}

void UIPanel::update()
{

}

void UIPanel::refresh()
{
	children.clear();

	if (!shaders)
		return;//look at how shaders is initialized


	for (int i = 0; i < shaders->size(); ++i)
	{
		addChild(std::make_unique<ShaderView>(x, y, w, 100, (*shaders)[i].get()));
	}
	for (int i = 1; i < children.size(); i++)
	{
		int newY = children[i - 1]->getY() + children[i - 1]->getH();
		children[i]->setPosition(children[i]->getX(), newY);
	}
}