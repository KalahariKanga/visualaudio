#pragma once
#include "Parameter.h"
#include <memory>
class UIElement
{
	static bool loadedFont;
	static sf::Font font;
protected:
	sf::RenderTexture* texture;
	std::vector<std::unique_ptr<UIElement>> children;
	UIElement* parent;
	void addChild(std::unique_ptr<UIElement> child);
	int x, y;
	int w, h;
public:
	UIElement(int x, int y, int w, int h);
	virtual ~UIElement();
	virtual void update() = 0;
	sf::Texture getTexture() { return texture->getTexture(); }
	int getX() { return x; }
	int getY() { return y; }
	void setPosition(int _x, int _y){ x = _x; y = _y; }
	void setSize(int _w, int _h){ w = _w; h = _h; }
	void resize(int _w, int _h);
	sf::Font* getFont();
};