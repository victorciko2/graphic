#include <iostream>
#include "sphere.h"

using namespace std;

Sphere::Sphere(){
	this->center = Point(0,0,0);
	this->referencePoint = Point(0,0,0);
	this->axis = Direction(0,0,0);
}

Sphere::Sphere(Point center, Point referencePoint, Direction axis){
	this->center = center;
	this->referencePoint = referencePoint;
	this->axis = axis;
}

void Sphere::setCenter(Point center){
	this->center = center; 
}

void Sphere::setReferencePoint(Point referencePoint){
	this->referencePoint = referencePoint;
}

void Sphere::setDirection(Direction axis){
	this->axis = axis;
}

Point Sphere::getCenter(){
	return this->center;
}

Point Sphere::getReferencePoint(){
	return this->referencePoint;
}

Direction Sphere::getAxis(){
	return this->axis;
}

void Sphere::show(){
	cout << "Center: " << flush;
	this->center.show(); 
	cout << endl << "ReferencePoint: " << flush;
	this->referencePoint.show();
	cout << endl << "Axis: " << flush;
	this->axis.show();
}

Sphere Sphere::operator=(Sphere s){
	this->center = s.getCenter();
	this->referencePoint = s.getReferencePoint();
	this->axis = s.getAxis();
	return *this;
}
