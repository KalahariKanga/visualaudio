#include "ShaderListView.h"
#include "UIButton.h"

ShaderListView::ShaderListView(int x, int y, int w, int h, ShaderList* shaders) : UIElement(x,y,w,h)
{
	shaderList = shaders;
	for (int i = 0; i < shaderList->size(); ++i)
	{
		addChild<ShaderView>(x, y, w, 16, shaderList->getShader(i));
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
	children.clear();
	for (int i = 0; i < shaderList->size(); ++i)
	{
		addChild<ShaderView>(x, y, w, 16, shaderList->getShader(i));
	}
	repositionChildren();
}

void ShaderListView::processEvent(sf::Event ev)
{

}

void ShaderListView::addShader(std::string name)
{
	shaderList->addShader(name);
	addChild<ShaderView>(x, y, w, 16, shaderList->getShader(shaderList->size()-1));
	requestRefresh();
}

void ShaderListView::removeShader(ShaderView* sh)
{
	shaderList->removeShader(sh->getShader());
	//removeChild((UIElement*)sh);
	needRefresh = 1;
}

void ShaderListView::moveShader(ShaderView* sh, int delta)
{
	shaderList->moveShader(sh->getShader(), delta);
	needRefresh = 1;
}