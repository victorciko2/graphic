#ifndef COLOR_H
#define COLOR_H
#include <iostream>

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

	float operator[](int index);
};

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