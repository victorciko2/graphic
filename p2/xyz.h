#ifndef XYZ_H
#define XYZ_H
#include <iostream>
#include "rgb.h"
#include "xyy.h"

using namespace std;

class RGB;

class xyY;

class XYZ{
private:
	float X, Y, Z;

public:
	XYZ();

	XYZ(float X, float Y, float Z);

	void setX(float X);

	void setY(float Y);

	void setZ(float Z);

	float getX();

	float getY();

	float getZ();

	RGB toRGB();

	xyY toxyY();
};

#endif