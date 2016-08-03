#pragma once
#include "Generator.h"

class Particle
{
public:
	Particle()
	{
		static int count = 0;
		id = count;
		++count;
		x = Math::random(0, 640);
		y = Math::random(0, 480);
		dir = Math::random(0, 2 * PI);
		spd = Math::random(0, 2);
	}
	~Particle()
	{
		x = 0;
	}
	float x, y, dir, spd;
	int id;
	void update()
	{
		x += spd*cos(dir);
		y += spd*sin(dir);
		if (x > 640)x = 0;
		if (x < 0)x = 640;
		if (y > 480)y = 0;
		if (y < 0)y = 480;
	}

};

class Gen_Swarm :
	public Generator
{
	
	int noParts = 30;
public:
	Gen_Swarm(AudioCapture* AC);
	~Gen_Swarm();
	virtual void update(Canvas& target);
	std::vector<Particle> particles;
};

