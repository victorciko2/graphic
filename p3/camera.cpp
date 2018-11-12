#include <iostream>
#include <cstring>
#include "camera.h"

Camera::Camera(){
	this->o = Point(0, 0, 0);
	this->c = CoordinateSystem();
	c.setI(Direction(1, 0, 0));
	c.setJ(Direction(0, 1, 0));
	c.setK(Direction(0, 0, 1));
	c.setO(o);
	this->x = 480;
	this->y = 480;
}

Camera::Camera(Point o, CoordinateSystem c, long int x, long int y){
	this->o = o;
	this->c;
	this->x = x;
	this->y = y;
}

Point Camera::getOrigin(){
	return this->o;
}

CoordinateSystem Camera::getCoordinateSystem(){
	return this->c;
}

long int Camera::getX(){
	return this->x;
}

long int Camera::getY(){
	return this->y;
}

void Camera::setOrigin(Point o){
	this->o = o;
}

void Camera::setCoordinateSystem(){
	this->c = c;
}

void Camera::setX(long int x){
	this->x = x;
}

void Camera::setY(long int y){
	this->y = y;
}

string Camera::showAsString(){
	string result = "CAMERA: \n" + this->c.showAsString() + "\n" + this->o.showAsString() + " x = " + to_string(this->x) + " y = " + to_string(this->y) + "\n";
	return result;
}

void Camera::show(){
	cout << this->showAsString() << endl;
}