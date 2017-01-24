#pragma once
#include "ParameterListView.h"
#include "ShaderList.h"
class ShaderView :
	public UIElement
{
	Shader* shader;
	std::vector<Parameter*> list;
	sf::Text name;
	void remove();
	void move(int delta);
public:
	ShaderView(int x, int y, int w, int h, Shader* shader);
	~ShaderView();
	virtual void update();
	virtual void refresh();
	virtual void processEvent(sf::Event ev);
	virtual void triggerCollapse() override;

	Shader* const getShader(){ return shader; }
};

