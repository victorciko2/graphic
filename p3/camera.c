#include <iostream>
#include <cstring>
#include "camera.h"

Camera::Camera(){
	this->o = new Point(0, 0, 0);
	this->c = new CoordinateSystem();
	c.setI(new Direction(1, 0, 0));
	c.setJ(new Direction(0, 1, 0));
	c.setK(new Direction(0, 0, 1));
	c.setO(o);
}

Camera::Camera(Point o, CoordinateSystem c){
	this->o = o;
	this->c;
}

Point Camera::getOrigin(){
	return this->o;
}

CoordinateSystem Camera::getCoordinateSystem(){
	return this->c;
}

void Camera::setOrigin(Point o){
	this->o = o;
}

void Camera::setCoordinateSystem(){
	this->c = c;
}

string Camera::showAsString(){
	string result = "CAMERA: \n" + this->c.showAsString() + "\n" + this->o.showAsString();
}

void Camera::show(){
	cout << this->showAsString() << endl;
}