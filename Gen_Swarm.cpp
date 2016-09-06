#include "Gen_Swarm.h"


Gen_Swarm::Gen_Swarm(AudioCapture* AC) : Generator(AC)
{
	addParameter("noParts", 30, 0, 300);
	addParameter("ai", 0, 0, 5, 1);
	particles.resize(noParts);
}


Gen_Swarm::~Gen_Swarm()
{
}

void Gen_Swarm::update(Canvas& target)
{
	int width = target.getWidth();
	int height = target.getHeight();
	auto param = getParameter("noParts");
	if (param->hasChanged())
	{
		particles.resize((int)param->getValue());
	}
	for (auto& p : particles)
	{
		if (p.x > width)p.x = 0;
		if (p.x < 0)p.x = width;
		if (p.y > height)p.y = 0;
		if (p.y < 0)p.y = height;

		switch ((int)getParameter("ai")->getValue())
		{
		case 0:
			p.spd = 8;
			p.dir = Math::point_direction(p.x, p.y, 0, 0) + Math::point_direction(p.x, p.y, 640, 480) + float(p.id)/30;
			break;
		case 1:
			p.spd = ac->getAmplitude() / 5;
			p.dir = ac->getAmplitude() / 5 + Math::point_direction(p.x, p.y, 0, 0);
			break;
		case 2:
			p.spd = 4;
			{
				short direction = p.id % 2 == 0 ? 1 : -1;
				p.dir = Math::point_direction(p.x, p.y, width / 2, height / 2 + ac->getAmplitude() * 5 * direction) + (PI / 2)*direction;
			}
			break;
		case 3://this one is awesome
			p.spd = ac->getAmplitude() / 5;
			p.dir = 0.3 + Math::point_direction(p.x, p.y, particles[(p.id + 1) % particles.size()].x, particles[(p.id + 1) % particles.size()].y);
			break;
		}
		p.update();
		target.setDrawColour(p.x/target.getWidth());
		target.drawCircle(p.x, p.y, ac->getAmplitude()/3, 0);
	}
}
