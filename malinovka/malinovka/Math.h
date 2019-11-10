#pragma once
#include <math.h>

class Math
{
public:
	static bool lineEquation(float x, float y, float x1, float y1, float x2, float y2)
	{
		return (y1 - y2)*x + (x2 - x1) + (x1*y1 - x2 * y2) == 0;
	}
	static float dist(float x1, float y1, float z1, float x2, float y2, float z2)
	{
		return sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1) + (z2 - z1)*(z2 - z1));
	}
	static float AngleBetweenTwoVectors(float x1, float y1, float z1, float x2, float y2, float z2) 
	{
		float fCrossX = y1 * z2 - z1 * y2;
		float fCrossY = z1 * x2 - x1 * z2;
		float  fCrossZ = x1 * y2 - y1 * x2;
		float fCross = sqrt(fCrossX * fCrossX +
			fCrossY * fCrossY + fCrossZ * fCrossZ);
		float fDot = x1 * x2 + y1 * y2 + z1 + z2;
		return atan2(fCross, fDot);
	}

	Math();
	~Math();
};