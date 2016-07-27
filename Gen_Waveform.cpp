#include "Gen_Waveform.h"


Gen_Waveform::Gen_Waveform(AudioCapture* ac) : Generator(ac)
{
}


Gen_Waveform::~Gen_Waveform()
{
}

void Gen_Waveform::update(Canvas& target)
{
	for (int cx = 0; cx < target.getWidth(); cx++)
	{
		int y1 = ac->getWaveform((float)cx / target.getWidth());
		int y2 = ac->getWaveform((float)(cx + 1) / target.getWidth());
		target.setDrawColour((float)abs(y1) / (target.getHeight() / 2));
		target.drawLine(cx, y1 + target.getHeight() / 2, cx + 1, y2 + target.getHeight() / 2);
	}
}