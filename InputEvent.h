#pragma once
struct InputButton
{
	enum class Device
	{
		Keyboard, Mouse, Gamepad, MIDI
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
};

struct InputEvent
{
	InputButton button;
	float data = 1;
	InputEvent(InputButton _button, float _data = 1) : button(_button), data(_data)
	{

	};
};
