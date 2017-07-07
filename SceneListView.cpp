#include "SceneListView.h"
#include "UISlider.h"
#include "UIButton.h"

using namespace UIStyle::Layout;

SceneListView::SceneListView(int x, int y, int w, int h, SceneList* list_) : UIElement(x, y, w, h), list(list_)
{
	std::function<void(void)> prevScene =	[&](){list->prevScene(); };
	std::function<void(void)> nextScene =	[&](){list->nextScene(); };
	std::function<void(void)> addScene =	[&](){list->addScene("Waveform"); };
	std::function<void(void)> removeScene = [&](){list->removeScene(); squashEvent(); };
	addChild<UIButton>(x + hPad, y + hPad, buttonSize, buttonSize, prevScene, "<");
	addChild<UISlider>(x + 2 * hPad + buttonSize, y + hPad, w - 6 * hPad - 4 * buttonSize, sliderH, list->getParameter("scene"));
	addChild<UIButton>(x + w - 3 * hPad - 3 * buttonSize, y + hPad, buttonSize, buttonSize, nextScene, ">");
	addChild<UIButton>(x + w - 2 * hPad - 2 * buttonSize, y + hPad, buttonSize, buttonSize, addScene, "+");
	addChild<UIButton>(x + w - hPad - buttonSize, y + hPad, buttonSize, buttonSize, removeScene, "x");
}


SceneListView::~SceneListView()
{
}

void SceneListView::update()
{

}