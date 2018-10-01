#include <iostream>
#include <cmath>
#include "direction.h"

using namespace std;

Direction::Direction(){
	this->x = 0;
	this->y = 0;
	this->z = 0;
}
Direction::Direction(float x, float y, float z){
	this->x = x;
	this->y = y;
	this->z = z;
}

void Direction::setX(float x){
	this->x = x;
}

void Direction::setY(float y){
	this->y = y;
}

void Direction::setZ(float z){
	this->z = z;
}

float Direction::getX(){
	return this->x;
}

float Direction::getY(){
	return this->y;
}

float Direction::getZ(){
	return this->z;
}

void Direction::show(){
	cout << "{ " << this->x << ", " << this->y << ", " << this->z << "}" << flush;
}

Point Direction::operator+(Point p){
	return Point(this->x + p.getX(), this->y + p.getY(), this->z + p.getZ());
}

Direction Direction::operator+(Direction d2){
	return Direction(this->x + d2.getX(), this->y + d2.getY(), this->z + d2.getZ());
}

Direction Direction::operator-(Direction d2){
	return Direction(this->x - d2.getX(), this->y - d2.getY(), this->z - d2.getZ());
}

Direction Direction::operator*(float s){
	return Direction(this->x * s, this->y * s, this->z * s);
}

Direction Direction::operator/(float s){
	return Direction(this->x / s, this->y / s, this->z / s);
}

Direction Direction::operator=(Direction d){
	this->x = d.getX();
	this->y = d.getY();
	this->z = d.getY();
	return *this;
}

float Direction::modulus(){
	return sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
}

float dotProduct(Direction d1, Direction d2){
	return d1.getX() * d2.getX() + d1.getY() * d2.getY() + d1.getZ() * d2.getZ();
}

Direction crossProduct(Direction d1, Direction d2){
	return Direction(d1.getY() * d2.getZ() - d1.getZ() * d2.getY(),
			d1.getZ() * d2.getX() - d1.getX() * d2.getZ(),
			d1.getX() * d2.getY() - d1.getY() * d2.getX());
}
