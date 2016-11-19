#pragma once
#include "Parameter.h"
#include <memory>
class UIElement
{
	static bool loadedFont;
	static sf::Font font;
protected:
	
	std::vector<std::unique_ptr<UIElement>> children;
	UIElement* parent;
	void addChild(std::unique_ptr<UIElement> child);
	int x, y;
	int w, h;
	bool active = 1;
	
public:
	UIElement(int x, int y, int w, int h);
	virtual ~UIElement();

	virtual void processEvent(sf::Event ev);
	virtual void update() = 0;
	virtual void refresh();

	void distributeEvent(sf::Event ev);
	void doUpdate();
	void doRefresh();

	sf::Texture getTexture() { return texture->getTexture(); }
	int getX() { return x; }
	int getY() { return y; }
	int getH();
	int getW();
	bool isActive() { return active; }
	void setActive(bool a) { active = a; }
	void setPosition(int _x, int _y);
	void offsetPosition(int dx, int dy);
	void setSize(int _w, int _h){ w = _w; h = _h; }
	void resize(int _w, int _h);
	void requestRefresh(){
		if (parent)
			parent->requestRefresh();
		else
			doRefresh();
	}
	
	sf::Font* getFont();

	static sf::RenderTexture* texture;//protect
};