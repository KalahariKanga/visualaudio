#include "PopupWindow.h"


PopupWindow::PopupWindow(int w, int h) : UIElement(0,0,w,h)
{
}


PopupWindow::~PopupWindow()
{
}

void PopupWindow::update()
{
	texture->display();
	window.draw(sf::Sprite(texture->getTexture()));
	window.display();
}