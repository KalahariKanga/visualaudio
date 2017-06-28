#include "ComboBoxPanel.h"

using namespace UIStyle::Colour;
using namespace UIStyle::Layout;

ComboBoxPanel::ComboBoxPanel(int w, int h, UIComboBox* box, sf::RenderTexture* tex) : UISubPanel(w, h), box(box)
{
	texture = tex;

	text.setFillColor(Primary);
	text.setFont(*UIElement::getFont());
	text.setCharacterSize(UIStyle::Text::fontSize);

	list = box->getItems();
}


ComboBoxPanel::~ComboBoxPanel()
{
}

void ComboBoxPanel::update()
{
	notNew = 1;//still dumb
	for (int c = 0; c < list.size(); c++)
	{
		text.setString(list[c]);
		text.setPosition(sf::Vector2f(x + 16, y + c * 32));
		draw(text);
	}
}

void ComboBoxPanel::refresh()
{
}

void ComboBoxPanel::processEvent(sf::Event ev)
{
	if (ev.type == sf::Event::MouseButtonPressed)
	{
		if (Math::pointInRect(ev.mouseButton.x, ev.mouseButton.y, x, y, x + w, y + h))
		{
			int i = ev.mouseButton.y / 32;
			try
			{
				box->setItem(box->getItems().at(i));
				toQuit = 1;
			}
			catch (...)
			{
				//lol
			}
		}
		else
		{
			if (notNew)
				toQuit = 1;
		}
	}
}