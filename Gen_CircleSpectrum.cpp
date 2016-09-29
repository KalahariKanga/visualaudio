#include "Gen_CircleSpectrum.h"


Gen_CircleSpectrum::Gen_CircleSpectrum(AudioCapture* AC) : Generator(AC)
{
	bandAmount.resize(bands,0);
	addParameter("decay", 0.04, 0, 1);
	addParameter("bands", 24, 1, 256);
	
	addParameter("burst", 0, 0, 0);
}


Gen_CircleSpectrum::~Gen_CircleSpectrum()
{
}

void Gen_CircleSpectrum::update(Canvas& target)
{
	if (getParameter("bands")->hasChanged())
	{
		bands = (int)getParameter("bands")->getValue();
		bandAmount.resize(bands);
	}
	for (int b = 0; b < bands; b++)
	{
		if (abs(ac->getFFT((float)b / bands,(float)(b+1)/bands)) > bandAmount[b])
			bandAmount[b] = abs(ac->getFFT((float)b / bands, (float)(b + 1) / bands));
		else
			bandAmount[b] -= getParameter("decay")->getValue();

		bandAmount[b] = Math::clamp(bandAmount[b], 0, 1);
	}
	if (getParameter("burst")->hasChanged())
	{
		for (int b = 0; b < bands; b++)
			bandAmount[b] = 1;
	}
	float innerRadius = ac->getAmplitude();
	for (int b = 0; b < bands-1; b++)
	{
		float a1 = PI * ((float)b / bands);
		float a2 = PI * ((float)(b+1) / bands);
		target.setDrawColour((float)b / bands);
		int x1 = (innerRadius + bandAmount[b] * 256) * sin(a1);
		int x2 = (innerRadius + bandAmount[b+1] * 256) * sin(a2);
		int y1 = (innerRadius + bandAmount[b] * 256) * cos(a1);
		int y2 = (innerRadius + bandAmount[b+1] * 256) * cos(a2);
		target.drawLine(target.getWidth() / 2 + x1, target.getHeight() / 2 + y1, target.getWidth() / 2 + x2, target.getHeight() / 2 + y2);
		target.drawLine(target.getWidth() / 2 - x1, target.getHeight() / 2 + y1, target.getWidth() / 2 - x2, target.getHeight() / 2 + y2);
		target.drawCircle(target.getWidth() / 2 + 2 * x1, target.getHeight() / 2 + 2 * y1, ac->getAmplitude() / 10, 1);
		target.drawCircle(target.getWidth() / 2 - 2 * x1, target.getHeight() / 2 + 2 * y1, ac->getAmplitude() / 10, 1);
	}
}