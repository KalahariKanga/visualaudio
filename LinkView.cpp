#include "LinkView.h"


LinkView::LinkView(int x, int y, int w, int h, InputButton button, Action action) : UIElement(x, y, w, h), button(button), action(action)
{
	deviceName.setString(button.getDeviceName()+" "+std::to_string((int)button.button));
	deviceName.setFillColor(sf::Color::White);
	deviceName.setFont(*UIElement::getFont());
	deviceName.setCharacterSize(12);

}


LinkView::~LinkView()
{
}

void LinkView::update()
{
	draw(deviceName);
	//draw(actionTarget);
}

void LinkView::refresh()
{
	deviceName.setPosition(x + 4, y + 4);
}