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

void InputMap::clear()
{
	map.clear();
}

void InputMap::merge(InputMap other)
{
	for (auto &i : other.map)
	{
		map.insert(i);
	}
}

void InputMap::updateLink(std::pair<InputButton, Action> from, std::pair<InputButton, Action> to)
{
	///updates the button portion of a link
	///not as flexible as it could be
	auto range = map.equal_range(from.first);

	std::vector<MapType::iterator> toDelete;

	for (auto i = range.first; i != range.second; ++i)
	{
		if (i->second.getTarget() == from.second.getTarget())
		{
			toDelete.push_back(i);
		}
	}
	if (!toDelete.empty())
		map.insert(to);//dont insert into both maps
	for (auto i : toDelete)
		map.erase(i);
	
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