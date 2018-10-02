#include <iostream>
#include <cstring>
#include "coordinateSystem.h"

using namespace std;

CoordinateSystem::CoordinateSystem(){
	this->i = Direction(1, 0, 0);
	this->j = Direction(0, 1, 0);
	this->k = Direction(0, 0, 1);
	// punto o
}

CoordinateSystem::CoordinateSystem(Direction i, Direction j, Direction k){
	this->i = i;
	this->j = j;
	this->k = k;
}

void CoordinateSystem::setI(Direction i){
	this->i = i;
}

void CoordinateSystem::setJ(Direction j){
	this->j = j;
}

void CoordinateSystem::setK(Direction k){
	this->k = k;
}

Direction CoordinateSystem::getI(){
	return this->i;
}

Direction CoordinateSystem::getJ(){
	return this->j;
}

Direction CoordinateSystem::getK(){
	return this->k;
}

string CoordinateSystem::showAsString(){
	string s = "{ " + this->i.showAsString() + ", " + this->j.showAsString()
		+ ", " + this->k.showAsString() + "}";
	return s;
}

void CoordinateSystem::show(){
	cout << this->showAsString() << endl;
}

CoordinateSystem CoordinateSystem::operator=(CoordinateSystem c){
	this->i = c.getI();
	this->j = c.getJ();
	this->k = c.getK();
	return *this;
}

