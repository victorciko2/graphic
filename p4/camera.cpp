#include <iostream>
#include <cstring>
#include "camera.h"

Camera::Camera(){
	this->o = Point(0, 0, 0);
	this->l = Direction(-1, 0, 0);
	this->u = Direction(0, 1, 0);
	this->f = Direction(0, 0, 1);
	this->x = 720;
	this->y = 480;
}

Camera::Camera(Point o, Direction f, Direction l, Direction u, long int x){
	this->o = o;
	this->x = x;
	this->f = f;
	this->l = l;
	this->u = u;
	this->y = this->u.modulus() * this->x / this->l.modulus();
}

Camera::Camera(Point o, Direction f, Direction l, long int x, long int y){
	this->f = f;
	this->u = Direction(0, (l.getX() * y) / x, 0);
	this->l = l;
	this->x = x;
	this->y = y;
	this->o = o;
}

Point Camera::getOrigin(){
	return this->o;
}

long int Camera::getX(){
	return this->x;
}

long int Camera::getY(){
	return this->y;
}

Direction Camera::getL(){
	return this->l;
}

Direction Camera::getF(){
	return this->f;
}

Direction Camera::getU(){
	return this->u;
}

void Camera::setL(Direction l){
	this->l = l;
}

void Camera::setF(Direction f){
	this->f = f;
}

void Camera::setU(Direction u){
	this->u = u;
}

void Camera::setOrigin(Point o){
	this->o = o;
}

void Camera::setX(long int x){
	this->x = x;
}

void Camera::setY(long int y){
	this->y = y;
}

string Camera::showAsString(){
	string result = "CAMERA: \nl:" + this->l.showAsString() + "\nu: " + this->u.showAsString() + "\nf: " + this->f.showAsString() 
			+ "\n" + this->o.showAsString() + " x = " + to_string(this->x) + " y = " + to_string(this->y) + "\n";
	return result;
}

void Camera::show(){
	cout << this->showAsString() << endl;
}