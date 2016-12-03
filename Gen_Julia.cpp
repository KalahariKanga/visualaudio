#include "Gen_Julia.h"


Gen_Julia::Gen_Julia(AudioCapture* AC) : Generator(AC)
{
	addParameter("minX", -2, -2, 2);
	addParameter("minY", 0, -2, 2);
	addParameter("maxX", 0, -2, 2);
	addParameter("maxY", 0, -2, 2);
}


Gen_Julia::~Gen_Julia()
{
}

void Gen_Julia::update(Canvas& target)
{
	float jx = Math::lint(getParameter("minX")->getValue(), getParameter("maxX")->getValue(), ac->getAmplitude() / 5);
	float jy = Math::lint(getParameter("minY")->getValue(), getParameter("maxY")->getValue(), ac->getAmplitude() / 5);

	for (int cx = 0; cx < target.getWidth(); cx++)
	{
		for (int cy = 0; cy < target.getHeight(); cy++)
		{

			bool draw = 1;
			float newa, newb;
			float a = -2 + ((float)cx / target.getWidth()) * 4;
			float b = -2 + ((float)cy / target.getHeight()) * 4;;
			int loopSize = 20;
			for (int count = 0; count < loopSize; count++)
			{
				newa = a*a - b*b + jx;
				newb = 2 * a*b + jy;

				if (newa > 2 || newa < -2 || newb > 2 || newb < -2)
				{
					draw = 0;
					break;
				}
				else
				{
					a = newa;
					b = newb;
					continue;
				}
			}
			if (draw)
			{
				target.setDrawColour(abs(newa + newb));
				target.drawPoint(cx, cy);
			}

		}
	}
}