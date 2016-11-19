#pragma once
#include "ParameterListView.h"
#include "Shader.h"
class ShaderView :
	public UIElement
{
	Shader* shader;
	std::vector<Parameter*> list;
	sf::Text name;
	bool collapsed = 0;
public:
	ShaderView(int x, int y, int w, int h, Shader* shader);
	~ShaderView();
	virtual void update();
	virtual void refresh();
	virtual void processEvent(sf::Event ev);
};

