#include "Gen_CircleWaveform.h"


Gen_CircleWaveform::Gen_CircleWaveform(AudioCapture* ac) : Generator(ac)
{
	addParameter("radius", 128, 0, 256);
	addParameter("width", 32, 0, 128);
	addParameter("mirror", 0, 0, 1, Parameter::Type::Switch);
	addParameter("sides", 128, 2, 256, Parameter::Type::Discrete);
}


Gen_CircleWaveform::~Gen_CircleWaveform()
{
}

void Gen_CircleWaveform::update(Canvas& target, float deltaTime)
{
	int w = target.getWidth();
	int h = target.getHeight();
	float radius = getParameter("radius")->getValue();
	float width = getParameter("width")->getValue();
	bool mirror = (bool)getParameter("mirror")->getValue();
	int sides = getParameter("sides")->getValue();
	for (int c = 0; c < sides; c++)
	{
		float r1 = radius + width * ac->getWaveform(c / (float)sides);
		float r2 = radius + width * ac->getWaveform((c + 1) / (float)sides);
		float t1 = (c / (float)sides) * 2 * PI;
		float t2 = ((c + 1) / (float)sides) * 2 * PI;
		target.setDrawColour(c / (float)sides);
		if (mirror)
		{
			target.drawLine(w / 2 + r1*sin(t1/2), h / 2 + r1*cos(t1/2), w / 2 + r2*sin(t2/2), h / 2 + r2*cos(t2/2));
			target.drawLine(w / 2 - r1*sin(t1/2), h / 2 + r1*cos(t1/2), w / 2 - r2*sin(t2/2), h / 2 + r2*cos(t2/2));
		}
		else
			target.drawLine(w / 2 + r1*sin(t1), h / 2 + r1*cos(t1), w / 2 + r2*sin(t2), h / 2 + r2*cos(t2));
	}

}