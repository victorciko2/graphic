#ifndef XYY_H
#define XYY_H
#include <iostream>
#include "rgb.h"
#include "xyz.h"

using namespace std;

class RGB;

class XYZ;

class xyY{
private:
	float x, y, yLum;
	
public:
	xyY();

	xyY(float x, float y, float yLum);

	float getX();

	float getY();

	float getYLum();

	void setX(float x);

	void setY(float y);

	void setYLum(float yLum);

	XYZ toXYZ();

	RGB toRGB();

};

#endif

