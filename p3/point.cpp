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

Point Point::operator*(CoordinateSystem c){
	array <array<float, 4>, 4> matrix = c.getMatrix();
	float vw =  this->x * matrix[0][3] + this->y * matrix[1][3] 
				 + this->z * matrix[2][3] + 1 * matrix[3][3];
	return Point((this->x * matrix[0][0] + this->y * matrix[1][0] 
				 + this->z * matrix[2][0] + 1 * matrix[3][0]) / vw,
				 (this->x * matrix[0][1] + this->y * matrix[1][1] 
				 + this->z * matrix[2][1] + 1 * matrix[3][1]) / vw,
				 (this->x * matrix[0][2] + this->y * matrix[1][2] 
				 + this->z * matrix[2][2] + 1 * matrix[3][2]) / vw);
}

bool Point::operator==(Point p){
	return this->x == p.getX() && this->y == p.getY() && this->z == p.getZ();
}
string Point::showAsString(){
	string p = "POINT:\n[" + to_string(this->x) + "," 
		+ to_string(this->y) + "," + to_string(this->z) + "]";
	return p;

}

float Point::operator[](int i){
	if(i == 0){
		return this->x;
	}
	else if(i == 1){
		return this->y;
	}
	else if(i == 2){
		return this->z;
	}
}


