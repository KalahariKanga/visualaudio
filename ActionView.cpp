#include "ActionView.h"


ActionView::ActionView(int x, int y, int w, int h, InputButton button, Action action) : UIElement(x, y, w, h), button(button), action(action)
{
	deviceName.setString(button.getDeviceName());
	deviceName.setFillColor(sf::Color::White);
	deviceName.setFont(*UIElement::getFont());
	deviceName.setCharacterSize(12);

	buttonName.setString(std::to_string((int)button.button));
	buttonName.setFillColor(sf::Color::White);
	buttonName.setFont(*UIElement::getFont());
	buttonName.setCharacterSize(12);

	/*actionType.setString(std::to_string((int)button.button));
	actionType.setFillColor(sf::Color::White);
	actionType.setFont(*UIElement::getFont());
	actionType.setCharacterSize(12);*/

}


ActionView::~ActionView()
{
}

void ActionView::update()
{
	draw(buttonName);
	draw(deviceName);
	//draw(actionTarget);
}

void ActionView::refresh()
{
	deviceName.setPosition(x + 4, y + 4);
	buttonName.setPosition(x + 20, y + 4);
}