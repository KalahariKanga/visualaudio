#include "Gen_Particles.h"


Gen_Particles::Gen_Particles(AudioCapture* AC) : Generator(AC)
{
	addParameter("outline", 0, 0, 1);
	addParameter("pattern", 3, 0, 5);
	addParameter("directionChange", 0, -0.1, 0.1);
	addParameter("spawnRate", 1, 0.1, 1);
	//triggers
	addParameter("reverse", 0, 0, 0);
	addParameter("split", 0, 0, 0);
	
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

void Gen_Particles::update(Canvas& target, float deltaTime)
{
	static int t = 0;
	static float direction = 1;
	++t;

	//bool reverse = getParameter("reverse")->hasChanged();
	//bool split = getParameter("split")->hasChanged();
	
	for (auto & p : particles)
	{
		if (p.x < -p.size || p.x > target.getWidth()+p.size || p.y < -p.size || p.y > target.getHeight()+p.size)
			p.active = 0;
		if (p.active)
		{
			p.update(deltaTime);
			p.direction += getParameter("directionChange")->getValue();
			/*if (reverse)
				p.direction += PI;
			if (split)
				p.direction += PI / 2 * ((int)p.x % 2 ? -1 : 1);*/
			target.setDrawColour(p.direction / (2 * PI));
			target.drawCircle(p.x, p.y, p.size, (int)getParameter("outline")->getValue());
		}
	}

	float rate = getParameter("spawnRate")->getValue();
	if (t % (int)((1/rate)) == 0)
	{
		switch ((int)getParameter("pattern")->getValue())
		{
		case 0:
			direction += (ac->getAmplitude() / 20) * (ac->getAmplitude() / 20);
			spawnParticle(target.getWidth() / 2, target.getHeight() / 2, direction, 6, ac->getAmplitude() * 4);
			spawnParticle(target.getWidth() / 2, target.getHeight() / 2, -direction, 6, ac->getAmplitude() * 4);
			break;
		case 1:
			direction += sin((float)t / 100);
			spawnParticle(target.getWidth() / 2, target.getHeight() / 2, direction, 2 + ac->getAmplitude(), ac->getAmplitude() * 4);
			spawnParticle(target.getWidth() / 2, target.getHeight() / 2, -direction, 2 + ac->getAmplitude(), ac->getAmplitude() * 4);
			break;
		case 2:
			direction += tan(sin((float)t / 200) / 2);
			spawnParticle(target.getWidth() / 2, target.getHeight() / 2, direction, 6, ac->getAmplitude() * 4);
			spawnParticle(target.getWidth() / 2, target.getHeight() / 2, -direction, 6, ac->getAmplitude() * 4);
			spawnParticle(target.getWidth() / 2, target.getHeight() / 2, direction + PI, 6, ac->getAmplitude() * 4);
			spawnParticle(target.getWidth() / 2, target.getHeight() / 2, -direction + PI, 6, ac->getAmplitude() * 4);
			break;
		case 3:
			direction += (ac->getAmplitude() / 6) * sin((float)t / 100);
			spawnParticle(target.getWidth() / 2, target.getHeight() / 2, direction, 6, ac->getAmplitude() * 4);
			spawnParticle(target.getWidth() / 2, target.getHeight() / 2, -direction, 6, ac->getAmplitude() * 4);
			break;
		}
	}

}