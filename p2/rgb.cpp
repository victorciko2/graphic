#include <iostream>
#include <cmath>
#include "rgb.h"

using namespace std;

RGB::RGB(){
	this->r = -1;
	this->g = -1;
	this->b = -1;
}

RGB::RGB(float r, float g, float b){
	this->r = r;
	this->g = g;
	this->b = b;
}

void RGB::setR(float r){
	this->r = r;
}

void RGB::setG(float g){
	this->g = g;
}

void RGB::setB(float b){
	this->b = b;
}

float RGB::getR(){
	return this->r;
}

float RGB::getG(){
	return this->g;
}

float RGB::getB(){
	return this->b;
}

XYZ RGB::toXYZ(){
	XYZ result;
	/*float r = this->r / 255.0;
	float g = this->g / 255.0;
	float b = this->b / 255.0;
	*/
	float r = this->r / 65535.0;
	float g = this->g / 65535.0;
	float b = this->b / 65535.0;
	

	r = ((r > 0.04045) ? powf((r + 0.055) / 1.055, 2.4) : (r / 12.92));
	g = ((g > 0.04045) ? powf((g + 0.055) / 1.055, 2.4) : (g / 12.92));
	b = ((b > 0.04045) ? powf((b + 0.055) / 1.055, 2.4) : (b / 12.92));
	r *= 100; g *= 100; b *= 100;
	result.setX(r * 0.4124564 + g * 0.3575761 + b * 0.1804375);
	result.setY(r * 0.2126729 + g * 0.7151522 + b * 0.0721750);
	result.setZ(r * 0.0193339 + g * 0.1191920 + b * 0.9503041);
	return result;
}

xyY RGB::toXYY(){
	XYZ xyz = this->toXYZ();
	xyY xyy = xyz.toxyY();
	return xyy;
}