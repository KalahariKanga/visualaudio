#include "Gen_Spirograph.h"


Gen_Spirograph::Gen_Spirograph(AudioCapture* ac) : Generator(ac)
{
	addParameter("ratio", 0.3, 0, 1);
	addParameter("offset", 0.6, 0, 2);
	addParameter("scale", 128, 1, 512);
	addParameter("speed", 0.1, 0, 1);
	addParameter("points", 1, 1, 16);
}


Gen_Spirograph::~Gen_Spirograph()
{
}

void Gen_Spirograph::update(Canvas& target, float deltaTime)
{
	float scale = getParameter("scale")->getValue();
	float k = getParameter("ratio")->getValue();
	float l = getParameter("offset")->getValue();
	float speed = getParameter("speed")->getValue() * (abs(ac->getAmplitude()) / 4) * deltaTime;
	int points = (int)getParameter("points")->getValue();

	for (float timeOffset = 0; timeOffset < speed; timeOffset += speed/10.0)
	{
		for (int c = 0; c < points; c++)
		{
			float t = time + timeOffset + 5*c;
			float x = scale * ((1 - k)*cos(t) + l * k * cos(((1 - k) / k)*t));
			float y = scale * ((1 - k)*sin(t) - l * k * sin(((1 - k) / k)*t));
		
			target.setDrawColour((float)c / points);
			float size = abs(ac->getWaveform((float)timeOffset / speed));
			target.drawCircle(target.getWidth() / 2 + x, target.getHeight() / 2 + y, size, 0);
		}
	}
	time += speed;
}