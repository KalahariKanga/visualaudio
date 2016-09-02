#include "Action.h"

Action::Action()
{

}
Action::Action(Parameter* _target, Action::Type _type, float _amount) : target(_target), type(_type), amount(_amount)
{
}


Action::~Action()
{
}

void Action::execute(float eventValue)
{
	switch (type)
	{
	case Type::set:
		target->setValue(amount);
		break;
	case Type::shift:
		target->setValue(target->getValue() + amount);
		break;
	case Type::trigger:
		if (target->getValue() > 0)
			target->setValue(0);
		else
			target->setValue(1);
		break;
	}
}