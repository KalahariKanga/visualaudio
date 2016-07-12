#include "Gen_Swarm.h"


Gen_Swarm::Gen_Swarm(AudioCapture* AC) : Generator(AC)
{
	addParameter("noParts", 30, 0, 300);
	addParameter("ai", 1, 5, 0);
	particles.resize(noParts);
}


Gen_Swarm::~Gen_Swarm()
{
}

void Gen_Swarm::update(Canvas& target)
{
	auto param = getParameter("noParts");
	if (param.hasChanged())
	{
		particles.resize((int)param.getValue());
	}
	for (auto& p : particles)
	{
		switch ((int)getParameter("ai").getValue())
		{
		case 0:
			p.spd = 8;
			p.dir = Math::point_direction(p.x, p.y, 0, 0) + Math::point_distance(p.x, p.y, 0, 0);
			break;
		case 1:
			p.spd = ac->getAmplitude() / 5;
			p.dir = ac->getAmplitude() / 5 + Math::point_direction(p.x,p.y,particles[0].x,particles[1].y);
			//p.dir = ac->getAmplitude() / 5 + Math::point_direction(p.x, p.y, 0, 0);
			break;
		}
		p.update();
		target.setDrawColour(target.pal->getColour(p.x/target.getWidth()));
		target.drawCircle(p.x, p.y, ac->getAmplitude()/3, 0);
	}
}
