#include <iostream>
#include <cmath>
#include <cstring>
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

float Direction::modulus(){
	return sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
}

void Direction::normalize(){
	float m = this->modulus();
	this->x /= m;
	this->y /= m;
	this->z /= m;
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

float Direction::operator[](int i){
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

Direction Direction::operator=(Direction d){
	this->x = d.getX();
	this->y = d.getY();
	this->z = d.getZ();
	return *this;
}

float Direction::operator*(Direction d){
	return this->x * d.getX() + this->y * d.getY() + this->z * d.getZ();
}

Direction Direction::operator^(Direction d){
	return Direction(this->y * d.getZ() - this->z * d.getY(),
			this->z * d.getX() - this->x * d.getZ(),
			this->x * d.getY() - this->y * d.getX());
}

string Direction::showAsString(){
	string d = "DIRECTION:\n[" + to_string(this->x) + "," 
			+ to_string(this->y) + "," + to_string(this->z) + "]";
	return d;
}

bool Direction::operator==(Direction d){
	return this->x == d.getX() && this->y == d.getY() && this->z == d.getZ();
}