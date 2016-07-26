#pragma once
#include <vector>
#include <algorithm>
#include "InputReciever.h"
#include "Canvas.h"
#include "AudioCapture.h"

class Generator : public InputReciever
{
	
protected:
	AudioCapture* const ac;
public:
	Generator(AudioCapture* AC);
	~Generator();
	virtual void update(Canvas& target);//draw

};

