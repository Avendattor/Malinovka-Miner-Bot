#pragma once
#include <math.h>

class zPOINT3
{
public:
	float x;
	float y;
	float z;

	static zPOINT3 init(float x, float y, float z)
	{
		zPOINT3 temp;
		temp.x = x;
		temp.y = y;
		temp.z = z;

		return temp;
	}

	void operator=(zPOINT3 & point)
	{
		this->x = point.x;
		this->y = point.y;
		this->z = point.z;
	}

	zPOINT3(float _x = 0, float _y = 0, float _z = 0) : x(_x), y(_y), z(_z)
	{
	
	}

	zPOINT3 operator-(zPOINT3 operand)
	{
		zPOINT3 temp;
		temp.x = this->x - operand.x;
		temp.y = this->y - operand.y;
		temp.z = this->z - operand.z;

		return temp;
	}
};

class zVEC3
{
public:
	
	zVEC3(zPOINT3 point)
	{
		this->x = point.x;
		this->y = point.y;
		this->z = point.z;
	}

	zVEC3(float _x = 0, float _y = 0, float _z = 0) : x(_x), y(_y), z(_z) {}

	void operator+(zVEC3 v)
	{
		this->x = this->x - v.x;
		this->y = this->y - v.y;
		this->z = this->z - v.z;
	}

	float x;
	float y;
	float z;
};

class Math
{
public:
	static bool lineEquation(float x, float y, float x1, float y1, float x2, float y2)
	{
		return (y1 - y2)*x + (x2 - x1) + (x1*y1 - x2 * y2) == 0;
	}

	static float dist(zPOINT3 & t1, zPOINT3 & t2)
	{
		return sqrt((t2.x - t1.x)*(t2.x - t1.x) + (t2.y - t1.y)*(t2.y - t1.y) + (t2.z - t1.z)*(t2.z - t1.z));
	}

	static float dist(float x1, float y1, float z1, float x2, float y2, float z2)
	{
		return sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1) + (z2 - z1)*(z2 - z1));
	}

	Math();
	~Math();
};