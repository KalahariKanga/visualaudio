#include "ShaderListView.h"


ShaderListView::ShaderListView(int x, int y, int w, int h, ShaderList* shaders) : UIElement(x,y,w,h)
{
	shaderList = shaders;
	for (int i = 0; i < shaderList->size(); ++i)
	{
		addChild(std::make_unique<ShaderView>(x, y, w, 16, shaderList->getShader(i)));
	}
}


ShaderListView::~ShaderListView()
{
}

void ShaderListView::update()
{

}

void ShaderListView::refresh()
{
	for (int i = 1; i < children.size(); i++)
	{
		int newY = children[i - 1]->getY() + children[i - 1]->getH();
		children[i]->setPosition(children[i]->getX(), newY);
	}
}

void ShaderListView::processEvent(sf::Event ev)
{

}