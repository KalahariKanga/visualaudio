#pragma once
#include "UIElement.h"
class UIComboBox :
	public UIElement
{
	std::vector<std::string> items;
	sf::RectangleShape outline;
	sf::Text text;
	std::string item;
	std::function<void(std::string)> callback;
public:
	UIComboBox(int x, int y, int w, int h, std::vector<std::string> list_, std::function<void(std::string)> callback);
	~UIComboBox();

	virtual void update() override;
	virtual void processEvent(sf::Event ev) override;
	virtual void refresh() override;

	const std::vector<std::string>& getItems();
	void setItem(std::string str);
	std::string getItem();
	static std::function<void(UIComboBox*)> popupCall;
};

