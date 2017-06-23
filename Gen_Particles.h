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
		float directionChange = 0, speedChange = 0;
		bool active = 1;
		void update(float deltaTime)
		{
			direction += directionChange;
			speedChange += speedChange;
			x += deltaTime * speed * cos(direction);
			y += deltaTime * speed * sin(direction);
		}
	};

	std::vector<Particle> particles;

public:
	Gen_Particles(AudioCapture* ac);
	~Gen_Particles();

	virtual void update(Canvas& target, float deltaTime);
	virtual std::string getName() override { return "Particles"; }

	void spawnParticle(float x, float y, float direction, float speed, float size);
};

