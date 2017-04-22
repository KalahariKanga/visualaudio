#include "Gen_Spectrum.h"


Gen_Spectrum::Gen_Spectrum(AudioCapture* ac) : Generator(ac)
{
}


Gen_Spectrum::~Gen_Spectrum()
{
}

void Gen_Spectrum::update(Canvas& target, float deltaTime)
{
	int width = target.getWidth();
	int height = target.getHeight();
	for (int cx = 0; cx < width; cx++)
	{
		float v = ac->getFFT((float)cx / width);
		target.setDrawColour(sqrt(abs(v)));
		target.drawLine(cx, (height / 2)*(1 - v), cx, (height / 2)*(1 + v));
	}
}