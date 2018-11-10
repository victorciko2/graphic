#include <iostream>
#include <cmath>
#include "xyz.h"

using namespace std;

XYZ::XYZ(){
	this->X = -1;
	this->Y = -1;
	this->Z = -1;
}

XYZ::XYZ(float X, float Y, float Z){
	this->X = X;
	this->Y = Y;
	this->Z = Z;
}

void XYZ::setX(float X){
	this->X = X;
}

void XYZ::setY(float Y){
	this->Y = Y;
}

void XYZ::setZ(float Z){
	this->Z = Z;
}

float XYZ::getX(){
	return this->X;
}

float XYZ::getY(){
	return this->Y;
}

float XYZ::getZ(){
	return this->Z;
}

RGB XYZ::toRGB(){
	float x = this->X / 100.0;
	float y = this->Y / 100.0;
	float z = this->Z / 100.0;

	float r = x * 3.2404542 + y * -1.5371385 + z * -0.4985314;
	float g = x * -0.9692660 + y * 1.8760108 + z * 0.0415560;
	float b = x * 0.0556434 + y * -0.2040259 + z * 1.0572252;

	r = ((r > 0.0031308) ? (1.055 * powf(r, 1 / 2.4) - 0.055) : (12.92 * r)) * 255.0;
	g = ((g > 0.0031308) ? (1.055 * powf(g, 1 / 2.4) - 0.055) : (12.92 * g)) * 255.0;
	b = ((b > 0.0031308) ? (1.055 * powf(b, 1 / 2.4) - 0.055) : (12.92 * b)) * 255.0;
	
	RGB color(r,g,b);

	return color;
}

xyY XYZ::toxyY(){
	float temp =this->X + this->Y + this->Z;
	float yLum = this->Y;
	float x = (temp == 0) ? 0 : (this->X / temp);
	float y = (temp == 0) ? 0 : (this->Y / temp);

	xyY result(x, y, yLum);

	return result;
}