#include "Action.h"

Action::Action()
{

}
Action::Action(Parameter* _target, Action::Type _type, float _amount, float _amount2) : target(_target), type(_type), amount(_amount), amount2(_amount2)
{
}


Action::~Action()
{
}

void Action::execute(float eventValue)
{
	if (!target)
	{
		std::cout << "Executing null action\n";
		return;
	}
	switch (type)
	{
	case Type::set:
		target->setValue(amount);
		break;
	case Type::setNormalised:
		target->setNormalisedValue(amount);
		break;
	case Type::shift:
		target->setValue(target->getValue() + amount);
		break;
	case Type::trigger:
		target->trigger();
		break;
	case Type::axis:
		target->setValue(amount + eventValue*(amount2-amount));
		break;
	}
}