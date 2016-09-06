#include "Gen_Waveform.h"


Gen_Waveform::Gen_Waveform(AudioCapture* ac) : Generator(ac)
{
	addParameter("fill", 0, 0, 1, 1);
	addParameter("yPosition", 0.5, 0, 1);
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

		int baseHeight = getParameter("yPosition")->getValue() * target.getHeight();

		if (getParameter("fill")->getValue() > 0)
			target.drawLine(cx, baseHeight, cx, y1 + baseHeight);
		else
			target.drawLine(cx, y1 + baseHeight, cx + 1, y2 + baseHeight);
	}
}