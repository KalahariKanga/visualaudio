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