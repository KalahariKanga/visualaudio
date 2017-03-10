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
	for (auto i : toDelete)
		map.erase(i);
	map.insert(to);
}

void InputMap::recieveEvent(InputEvent ev)
{
	auto range = map.equal_range(ev.button);

	for_each(range.first, range.second,
		[ev](MapType::value_type& x)
		{
			auto action = x.second;
			action.execute(ev.data);
		}
	);
}

InputMap::MapType InputMap::findParameterActions(Parameter* p)
{
	MapType actions;
	for (auto &i : map)
	{
		if (i.second.usesParameter(p))
			actions.insert(i);
	}
	return actions;
}