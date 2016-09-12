#include "Gen_Particles.h"


Gen_Particles::Gen_Particles(AudioCapture* AC) : Generator(AC)
{
}


Gen_Particles::~Gen_Particles()
{
}

void Gen_Particles::spawnParticle(float x, float y, float direction, float speed, float size)
{
	for (auto & p : particles)
	{
		if (!p.active)
		{
			p.active = 1;
			p.x = x;
			p.y = y;
			p.direction = direction;
			p.speed = speed;
			p.size = size;
			return;
		}
	}
	particles.emplace_back(x, y, direction, speed, size);
}

void Gen_Particles::update(Canvas& target)
{
	static int t = 0;
	static float direction = 0;
	++t;
	for (auto & p : particles)
	{
		if (p.x < -p.size || p.x > target.getWidth()+p.size || p.y < -p.size || p.y > target.getHeight()+p.size)
			p.active = 0;
		p.update();
		p.direction += 0.01;
		target.setDrawColour(p.direction / (2 * PI));
		target.drawCircle(p.x, p.y, p.size, 0);
	}
	direction += ac->getAmplitude() / 100;
	spawnParticle(target.getWidth() / 2, target.getHeight() / 2, direction, 3, ac->getAmplitude());
	spawnParticle(target.getWidth() / 2, target.getHeight() / 2, -direction, 3, ac->getAmplitude());

}