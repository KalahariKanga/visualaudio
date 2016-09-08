#include "Gen_CircleSpectrum.h"


Gen_CircleSpectrum::Gen_CircleSpectrum(AudioCapture* AC) : Generator(AC)
{
	bandAmount.resize(bands,0);
}


Gen_CircleSpectrum::~Gen_CircleSpectrum()
{
}

void Gen_CircleSpectrum::update(Canvas& target)
{
	for (int b = 0; b < bands; b++)
	{
		if (ac->getFFT((float)b / bands) > bandAmount[b])
			bandAmount[b] = abs(ac->getFFT((float)b / bands));
		else
			bandAmount[b] -= 0.04;
		bandAmount[b] = Math::clamp(bandAmount[b], 0, 1);
	}
	for (int b = 0; b < bands-1; b++)
	{
		float a1 = PI * ((float)b / bands);
		float a2 = PI * ((float)(b+1) / bands);
		target.setDrawColour((float)b / bands);
		int x1 = bandAmount[b] * 256 * sin(a1);
		int x2 = bandAmount[b + 1] * 256 * sin(a2);
		int y1 = bandAmount[b] * 256 * cos(a1);
		int y2 = bandAmount[b + 1] * 256 * cos(a2);
		target.drawLine(target.getWidth() / 2 + x1, target.getHeight() / 2 + y1, target.getWidth() / 2 + x2, target.getHeight() / 2 + y2);
		target.drawLine(target.getWidth() / 2 - x1, target.getHeight() / 2 + y1, target.getWidth() / 2 - x2, target.getHeight() / 2 + y2);
	}
}