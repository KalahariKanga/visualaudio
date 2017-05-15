#pragma once
#include "Parameter.h"
#include "UIStyle.h"
#include <memory>
#include <functional>
class UIElement
{
	static bool loadedFont;
	static sf::Font font;
protected:
	using ElementVector = std::vector < std::unique_ptr<UIElement> > ;
	ElementVector children;
	std::vector<UIElement*> toRemove;
	UIElement* parent;
	template <class T, class... Args> void addChild(Args&&... args);
	void removeChild(UIElement* elem);
	void repositionChildren();
	int x, y;
	int w, h;
	bool active = 1;
	bool collapsed = 0;
	bool needRefresh = 0;
public:
	UIElement(int x, int y, int w, int h);
	virtual ~UIElement();

	virtual void processEvent(sf::Event ev);
	virtual void update() = 0;
	virtual void refresh();

	void distributeEvent(sf::Event ev);
	void doUpdate();
	void doRefresh();

	sf::Texture getTexture();
	void draw(const sf::Drawable& src);
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
			needRefresh = 1;
	}
	virtual void triggerCollapse();
	sf::Font* getFont();

	sf::RenderTexture* texture;//protect
};

template <class T, class... Args>
void UIElement::addChild(Args&&... args)
{
	children.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
	children.back()->parent = this;
	children.back()->doRefresh();
}