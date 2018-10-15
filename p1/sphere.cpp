#include <iostream>
#include <cstring>
#include <cmath>
#include <stdlib.h>
#include "sphere.h"

using namespace std;

Sphere::Sphere(){
	this->center = Point();
	this->referencePoint = Point();
	this->axis = Direction();
	this->coordinates = CoordinateSystem();
}

Sphere::Sphere(Point center, Point referencePoint, Direction axis){
	this->center = center;
	this->referencePoint = referencePoint;
	this->axis = axis;
	Direction d = referencePoint - center;
	float modulus = d.modulus();
	if(abs(this->axis.modulus()/2.0 - modulus) >= pow(10, -6)){
		cout << "The distance between the center and the reference "
			 <<	"city is not the same as the axis divided by two." << endl;
	}
	Direction k = axis;
	k.normalize(); 
	Direction aux = this->referencePoint - this->center;
	Direction j = aux ^ k;
	j.normalize();
	Direction i = k ^ j;
	i.normalize();
	this->coordinates.setI(i);
	this->coordinates.setJ(j);
	this->coordinates.setK(k);
	this->coordinates.setO(this->center);
}

void Sphere::setCenter(Point center){
	this->center = center; 
}

void Sphere::setReferencePoint(Point referencePoint){
	this->referencePoint = referencePoint;
}

void Sphere::setAxis(Direction axis){
	this->axis = axis;
}

void Sphere::setCoordinates(CoordinateSystem coordinates){
	this->coordinates = coordinates;
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

CoordinateSystem Sphere::getCoordinates(){
	return this->coordinates;
}

string Sphere::showAsString(){
	string s = "SPHERE:\n Center: " + this->center.showAsString() + "\nReferencePoint: " 
			+ this->referencePoint.showAsString() 
			+ "\nAxis: " + this->axis.showAsString()
			+ "\n Coordinates:\n" + this->coordinates.showAsString();
	return s;
}

void Sphere::show(){
	cout << this->showAsString() << endl;
}

Sphere Sphere::operator=(Sphere s){
	this->center = s.getCenter();
	this->referencePoint = s.getReferencePoint();
	this->axis = s.getAxis();
	this->coordinates = s.getCoordinates();
	return *this;
}
