#include "InputEvent.h"

std::string InputButton::getDeviceName()
{
	switch (device)
	{
	case Device::None:
		return "None";
	case Device::Keyboard:
		return "Key";
	case Device::Mouse:
		return "Mouse";
	case Device::GamepadButton:
		return "Button";
	case Device::GamepadAxis:
		return "Axis";
	case Device::MIDINote:
		return "Note";
	case Device::MIDICV:
		return "CV";
	case Device::Audio:
		return "Audio";
	default:
		return "Unknown";
	}
}