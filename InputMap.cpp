#include "InputMap.h"


InputMap::InputMap()
{
}


InputMap::~InputMap()
{
}

void InputMap::addAction(InputButton input, Action action)
{
	map.insert(MapType::value_type(input, action));
}

void InputMap::recieveEvent(InputEvent ev)
{
	for (auto& a : map)
	{
		if (a.first.button == ev.button.button && a.first.device == ev.button.device)
		{
			auto action = a.second;
			action.execute(ev.data);
		}
	}
}

std::vector<std::pair<InputButton*, Action*>> InputMap::findParameterActions(Parameter* p)
{
	std::vector<std::pair<InputButton*, Action*>> actions;
	for (auto &i : map)
	{
		if (i.second.usesParameter(p))
		{
			auto item = std::make_pair(const_cast<InputButton*>(&i.first), &i.second);
			actions.push_back(item);
		}
	}
	return actions;
}

void InputMap::removeLink(std::pair<InputButton, Action> link)
{
	for (auto i = map.begin(); i != map.end(); i++)
	{
		//TODO assumes an eventbutton only affects a parameter in one way
		//enforce this or be better otherwise
		if (i->first.button == link.first.button && i->first.device == link.first.device)
		if (i->second.getTarget() == link.second.getTarget())
		{
			map.erase(i);
			break;
		}	
	}
}