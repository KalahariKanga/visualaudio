#pragma once
#include "UIElement.h"
#include "ShaderList.h"
#include "ShaderView.h"
class ShaderListView :
	public UIElement
{
	ShaderList* shaderList;
public:
	ShaderListView(int x, int y, int w, int h, ShaderList* shaders);
	~ShaderListView();
	virtual void update();
	virtual void processEvent(sf::Event ev);
	virtual void refresh();
	void addShader(std::string filename);
	void removeShader(ShaderView* sh);
	void moveShader(ShaderView* sh, int delta);
};

