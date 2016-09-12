#pragma once
#include "Generator.h"
class Gen_Particles :
	public Generator
{
	struct Particle
	{
		Particle(float _x, float _y, float _direction, float _speed, float _size) :
			x(_x), y(_y), direction(_direction), speed(_speed), size(_size){ };
		float x, y, direction, speed, size;
		bool active = 1;
		void update()
		{
			x += speed * cos(direction);
			y += speed * sin(direction);
		}
	};

	std::vector<Particle> particles;

public:
	Gen_Particles(AudioCapture* ac);
	~Gen_Particles();

	virtual void update(Canvas& target);
	void spawnParticle(float x, float y, float direction, float speed, float size);
};

