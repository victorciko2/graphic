#include <iostream>
#include <cstring>
#include "point.h"

using namespace std;

Point::Point(){
	this->x = 0;
	this->y = 0;
	this->z = 0;
}

Point::Point(float x, float y, float z){
	this->x = x;
	this->y = y;
	this->z = z;
}

void Point::setX(float x){
	this->x = x;
}

void Point::setY(float y){
	this->y = y;
}

void Point::setZ(float z){
	this->z = z;
}

float Point::getX(){
	return this->x;
}

float Point::getY(){
	return this->y;
}

float Point::getZ(){
	return this->z;
}

void Point::show(){
	cout << "{ " << this->x << ", " << this->y << ", " << this->z << "} " << flush;
}

Point Point::operator+(Direction d){
	return Point(this->x + d.getX(), this->y + d.getY(), this->z + d.getZ());
}

Direction Point::operator-(Point p2){
	return Direction(this->x - p2.getX(), this->y - p2.getY(), this->z - p2.getZ());
}

Point Point::operator=(Point p){
	this->x = p.getX(); 
	this->y = p.getY();
	this->z = p.getZ();
	return *this;
}

string Point::showAsString(){
	string p = "[" + to_string(this->x) + "," 
		+ to_string(this->y) + "," + to_string(this->z) + "]";
	return p;

}

