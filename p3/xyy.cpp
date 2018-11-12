#include <iostream>
#include "xyy.h"

using namespace std;

xyY::xyY(){
	this->x = -1;
	this->y = -1;
	this->yLum = -1;
}

xyY::xyY(float x, float y, float yLum){
	this->x = x;
	this->y = y;
	this->yLum = yLum;
}

float xyY::getX(){
	return this->x; 
}

float xyY::getY(){
	return this->y;
}

float xyY::getYLum(){
	return this->yLum;
}

void xyY::setX(float x){
	this->x = x;
}

void xyY::setY(float y){
	this->y = y;
}

void xyY::setYLum(float yLum){
	this->yLum = yLum;
}

XYZ xyY::toXYZ(){
	XYZ result;
	result.setX(this->x * (this->yLum / this->y));
	result.setY(this->yLum);
	result.setZ((1 - this->x - this->y) * (this->yLum / this->y));
	
	return result;
}

RGB xyY::toRGB(){
	XYZ result = this->toXYZ();
	RGB rgb = result.toRGB();
	
	return rgb;
}
