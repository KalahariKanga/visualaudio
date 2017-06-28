#pragma once
#include "ParameterListView.h"
#include "UIElement.h"
#include "Generator.h"
#include "UIButton.h"
#include "Scene.h"

class GeneratorView :
	public UIElement
{
	Generator* generator = nullptr;
	Scene* scene = nullptr;
	std::vector<Parameter*> list;
	sf::Text name;

	void rebuildChildren();//maybe
public:
	GeneratorView(int x, int y, int w, int h, Scene* scene);
	~GeneratorView();
	virtual void update();
	virtual void refresh();
	virtual void processEvent(sf::Event ev);
	virtual void triggerCollapse() override;
};

