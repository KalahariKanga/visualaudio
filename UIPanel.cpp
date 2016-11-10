#include "UIPanel.h"


UIPanel::UIPanel(int x, int y, int w, int h, std::vector<std::unique_ptr<Shader>>* shaders, Generator* gen) : UIElement(x, y, w, h)
{
	this->shaders = shaders;
	this->generator = gen;
	addChild(std::make_unique<GeneratorView>(x, y, w, 100, generator));
	for (int i = 0; i < shaders->size(); ++i)
	{
		addChild(std::make_unique<ShaderView>(x, y, w, 100, (*shaders)[i].get()));
	}
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

	for (int i = 1; i < children.size(); i++)
	{
		int newY = children[i - 1]->getY() + children[i - 1]->getH();
		children[i]->setPosition(children[i]->getX(), newY);
	}
}