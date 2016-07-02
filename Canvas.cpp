
#include "Canvas.h"


Canvas::Canvas()
{
	
}

Canvas::Canvas(int width, int height)
{
	drawColour = sf::Color(255, 0, 0);
	setDrawAlpha(255);
	data = new sf::Uint8[width*height*4];
	this->width = width;
	this->height = height;
	
}

Canvas::~Canvas()
{
}

void Canvas::colourToData(int x, int y, sf::Color c)
{
	data[4 * (y*width + x)] = c.r;
	data[4 * (y*width + x) + 1] = c.g;
	data[4 * (y*width + x) + 2] = c.b;
	data[4 * (y*width + x) + 3] = c.a;
}

void Canvas::loadFromFile(std::string fname)
{
	delete data;
	sf::Image i;
	i.loadFromFile(fname);
	
	width = i.getSize().x;
	height = i.getSize().y;
	data = new sf::Uint8[width * height * 4];
	const sf::Uint8* p = i.getPixelsPtr();
	for (int c = 0; c < width*height * 4; c++)
	{
		data[c] = *(p + c);
	}
}

sf::Color Canvas::getPoint(int x, int y)
{
	if (x < 0 || x >= width || y < 0 || y >= height)
		return sf::Color(0, 0, 0); //TODO: wrap or extend or something
	return sf::Color(data[4 * (y*width + x)], data[4 * (y*width + x) + 1], data[4 * (y*width + x) + 2], data[4 * (y*width + x) + 3]);
}

void Canvas::setDrawColour(sf::Color c)
{
	drawColour = c;
}

void Canvas::setDrawAlpha(float a)
{
	drawColour.a = a;//clamp(a*255, 0, 255);
}

void Canvas::drawPoint(int x, int y)
{
	drawPoint(x, y, drawColour);
}

void Canvas::drawPoint(int x, int y, sf::Color col)
{
	if (x < 0 || x >= width || y < 0 || y >= height)
		return;
	if (col.a == 0)
		return;
	if (col.a < 255)
	{
		sf::Color dest = getPoint(x, y);
		sf::Color out;
		float ca = (float)col.a / 255;
		float da = (float)dest.a / 255;
		float oa = ca + da*(1 - ca);
		if (oa > 0)
		{
			
			out.r = (col.r*ca + dest.r*da*(1 - ca)) / oa;
			out.g = (col.g*ca + dest.g*da*(1 - ca)) / oa;
			out.b = (col.b*ca + dest.b*da*(1 - ca)) / oa;
			out.a = oa * 255;
		}
		else
		{
			out.r = 0;
			out.g = 0;
			out.b = 0;
		}
		colourToData(x, y, out);
	}
	else
	{
		colourToData(x, y, col);
	}
}

void Canvas::drawLine(int x1, int y1, int x2, int y2)
{
	int dx = x2 - x1;
	int dy = y2 - y1;
	int dx1 = std::abs(dx);
	int dy1 = std::abs(dy);
	int px = 2 * dy1 - dx1;
	int py = 2 * dx1 - dy1;
	int x, y, xe, ye;
	if (dy1 <= dx1)
	{
		if (dx >= 0)
		{
			x = x1;
			y = y1;
			xe = x2;
		}
		else
		{
			x = x2;
			y = y2;
			xe = x1;
		}
		drawPoint(x, y, drawColour);
		for (int i = 0; x < xe; i++)
		{
			x = x + 1;
			if (px < 0)
			{
				px = px + 2 * dy1;
			}
			else
			{
				if ((dx < 0 && dy < 0) || (dx>0 && dy>0))
				{
					y = y + 1;
				}
				else
				{
					y = y - 1;
				}
				px = px + 2 * (dy1 - dx1);
			}
			drawPoint(x, y, drawColour);
		}
	}
	else
	{
		if (dy >= 0)
		{
			x = x1;
			y = y1;
			ye = y2;
		}
		else
		{
			x = x2;
			y = y2;
			ye = y1;
		}
		drawPoint(x, y, drawColour);
		for (int i = 0; y < ye; i++)
		{
			y = y + 1;
			if (py <= 0)
			{
				py = py + 2 * dx1;
			}
			else
			{
				if ((dx < 0 && dy < 0) || (dx>0 && dy>0))
				{
					x = x + 1;
				}
				else
				{
					x = x - 1;
				}
				py = py + 2 * (dx1 - dy1);
			}
			drawPoint(x, y, drawColour);
		}
	}
}

void Canvas::drawCircle(int x, int y, int r, bool outline)
{
	if (outline)
	{
		float jump = (2 * PI) / 32;
		for (int c = 0; c < 32; c++)
		{
			drawLine(x + r*cos(c*jump), y + r*sin(c*jump), x + r*cos((c + 1)*jump), y + r*sin((c + 1)*jump));
		}
	}
	else
	{
		for (int cx = x - r; cx < x + r; cx++)
			for (int cy = y - r; cy < y + r; cy++)
			{
				if ((cx - x)*(cx - x) + (cy - y)*(cy - y) < r*r)
					drawPoint(cx, cy);
			}
	}
}

void Canvas::drawRectangle(int x1, int y1, int x2, int y2, bool outline)
{
	if (outline)
	{
		drawLine(x1, y1, x2, y1);
		drawLine(x2, y1, x2, y2);
		drawLine(x1, y2, x2, y2);
		drawLine(x1, y1, x1, y2);
	}
	else
	{
		for (int cx = x1; cx <= x2; cx++)
			for (int cy = y1; cy <= y2; cy++)
				drawPoint(cx, cy);
	}
}

void Canvas::drawCanvas(Canvas* source, int x, int y)
{
	for (int cx = 0; cx < source->width; cx++)
		for (int cy = 0; cy < source->height; cy++)
		{
			drawPoint(x + cx, y + cy, source->getPoint(cx, cy));
		}
}
//
//void Canvas::drawCircleFunction(int x, int y, int r, std::function<float(float)> f)
//{
//	for (float theta = 0; theta <= 2 * PI; theta += PI / 8)
//	{
//		float r0 = f(theta);
//		float r1 = f(theta + PI / 8);
//		drawLine(x+(r+r0)*cos(theta), y+(r+r0)*sin(theta), x+(r+r1)*cos(theta + PI / 8), y+(r+r1)*sin(theta + PI / 8));
//	}
//}
//
//void Canvas::drawLineFunction(int x0, int y0, int x1, int y1, std::function<float(float)> f, bool fixEndpoints)
//{
//	auto fix = [](float p){return 1 - 4 * ((p - 0.5)*(p - 0.5)); };
//	//the unit vector perpendicular to the line
//	float l = sqrt((x1 - x0)*(x1 - x0) + (y1 - y0)*(y1 - y0));
//	float ux = (y0 - y1) / l;
//	float uy = (x1 - x0) / l;
//	
//	for (float t = 0; t < 1; t += 0.05)//optimise for length
//	{
//		//the point t of the way along the line
//		int x3 = x0 + (t+0.05)*(x1 - x0);
//		int y3 = y0 + (t+0.05)*(y1 - y0);
//		int x2 = x0 + t*(x1 - x0);
//		int y2 = y0 + t*(y1 - y0);
//
//		float o1 = f(t);
//		float o2 = f(t + 0.05);
//
//		if (fixEndpoints)
//		{
//			o1 *= fix(t);
//			o2 *= fix(t + 0.05);
//		}
//		drawLine(x2 + o1*ux, y2 + o1*uy, x3 + o2*ux, y3 + o2*uy);
//	}
//}

void Canvas::clear(sf::Color c)
{
	for (int i = 0; i < width* height; i++)
	{
		data[4 * i] = c.r;
		data[4 * i + 1] = c.g;
		data[4 * i + 2] = c.b;
	}
}
void Canvas::clear()
{
	clear(drawColour);
}