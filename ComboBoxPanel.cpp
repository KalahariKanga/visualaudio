#include "ComboBoxPanel.h"

using namespace UIStyle::Colour;
using namespace UIStyle::Layout;

ComboBoxPanel::ComboBoxPanel(int w, int h, UIComboBox* box, sf::RenderTexture* tex) : UISubPanel(w, h), box(box)
{
	texture = tex;

	text.setFillColor(Primary);
	text.setFont(*UIElement::getFont());
	text.setCharacterSize(UIStyle::Text::fontSize);

	outline.setOutlineThickness(1);
	outline.setFillColor(sf::Color::Transparent);
	outline.setSize(sf::Vector2f(w-1, 32 - 2 * hPad));//magic

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
		if (c == mouseOverId)
			outline.setOutlineColor(PrimaryHighlight);
		else
			outline.setOutlineColor(Primary);
		text.setString(list[c]);
		text.setPosition(sf::Vector2f(x + 16, y + 2 * hPad + c * 32));
		outline.setPosition(sf::Vector2f(x, y + hPad + c * 32));
		draw(outline);
		draw(text);
	}
}

void ComboBoxPanel::refresh()
{
}

void ComboBoxPanel::processEvent(sf::Event ev)
{
	if (ev.type == sf::Event::MouseMoved)
	{
		if (Math::pointInRect(ev.mouseMove.x, ev.mouseMove.y, x, y, x + w, y + h))
			mouseOverId = ev.mouseMove.y / 32;
		else
			mouseOverId = -1;
	}
	if (ev.type == sf::Event::MouseButtonPressed)
	{
		if (Math::pointInRect(ev.mouseButton.x, ev.mouseButton.y, x, y, x + w, y + h))
		{
			try
			{
				box->setItem(box->getItems().at(mouseOverId));
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