#pragma once
#include <string>
struct InputButton
{
	enum class Device
	{
		None, Keyboard, Mouse, GamepadButton, GamepadAxis, MIDINote, MIDICV, Audio
	} device;
	int button;
	bool operator<(const InputButton& other) const
	{
		if (device < other.device)
			return true;
		if (device > other.device)
			return false;
		return (button < other.button);
	}
	InputButton(Device _device, int _button) : device(_device), button(_button)
	{

	};
	std::string getDeviceName();
};

struct InputEvent
{
	InputButton button;
	float data = 1;
	InputEvent(InputButton _button, float _data = 1) : button(_button), data(_data)
	{

	};
	InputEvent(InputButton::Device _device, int _button, float _data = 1) : button(_device, _button), data(_data)
	{
		
	};
};

