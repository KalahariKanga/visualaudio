#pragma once
#include "UISubPanel.h"
#include "UIComboBox.h"

class ComboBoxPanel :
	public UISubPanel
{
	UIComboBox* box;
	std::vector<std::string> list;
	sf::Text text;
public:
	ComboBoxPanel(int w, int h, UIComboBox* box, sf::RenderTexture* tex);
	~ComboBoxPanel();

	virtual void update();
	virtual void processEvent(sf::Event ev);
	virtual void refresh();
};

