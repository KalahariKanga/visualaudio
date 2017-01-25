#include "Gen_Waveform.h"


Gen_Waveform::Gen_Waveform(AudioCapture* ac) : Generator(ac)
{
	addParameter("fill", 0, 0, 1);
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
		{
			int ny1 = Math::clamp(y1 + baseHeight, 0, target.getHeight());
			int ny2 = Math::clamp(y2 + baseHeight, 0, target.getHeight());
			target.drawLine(cx, ny1, cx + 1, ny2);
		}
	}
}