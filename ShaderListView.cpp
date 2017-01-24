#include "ShaderListView.h"
#include "UIButton.h"

ShaderListView::ShaderListView(int x, int y, int w, int h, ShaderList* shaders) : UIElement(x,y,w,h)
{
	shaderList = shaders;
	for (int i = 0; i < shaderList->size(); ++i)
	{
		addChild(std::make_unique<ShaderView>(x, y, w, 16, shaderList->getShader(i)));
	}
	//addChild(std::make_unique<UIButton>(x, y, 8, 8, [this](){addShader("shaders/bend"); }));
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

void ShaderListView::addShader(std::string filename)
{
	shaderList->addShader(filename);
	addChild(std::make_unique<ShaderView>(x, y, w, 16, shaderList->getShader(shaderList->size()-1)));
	requestRefresh();
}

void ShaderListView::removeShader(ShaderView* sh)
{
	shaderList->removeShader(sh->getShader());
	removeChild((UIElement*)sh);
	requestRefresh();
}

void ShaderListView::moveShader(ShaderView* sh, int delta)
{
	shaderList->moveShader(sh->getShader(), delta);
	
	
	auto it = std::find_if(children.begin(), children.end(),
		[sh](std::unique_ptr<UIElement>& elem){return (elem.get() == sh); });
	if (it != children.end())
	{
		int index = it - children.begin();
		try
		{
			std::swap(children.at(index), children.at(index + delta));
		}
		catch (...)
		{
			//im dreadful
		}
	}
	requestRefresh();
}