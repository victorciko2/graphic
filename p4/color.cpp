#include <iostream>
#include <cmath>
#include "color.h"

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

string RGB::showAsString(){
	string result = "R: " + to_string(this->r) + " G: " + to_string(this->g) + " B: " + to_string(this->b);
	return result;
}

void RGB::show(){
	cout << this->showAsString() << endl;
}

float RGB::operator[](int index){
    if(index == 0){
    	return r;
    }
    else if(index == 1){
    	return g;
    }
    else if(index == 2){
    	return b;
    }
}

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
