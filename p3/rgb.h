#ifndef RGB_H
#define RGB_H
#include <iostream>
#include "xyz.h"
#include "xyy.h"

using namespace std;

class XYZ;

class xyY;

class RGB{
private:
	float r, g, b;
public:
	RGB();

	RGB(float r, float g, float b);

	void setR(float r);

	void setG(float g);

	void setB(float b);

	float getR();

	float getG();

	float getB();

	XYZ toXYZ();

	xyY toXYY();

	string showAsString();
	
	void show();
};

#endif