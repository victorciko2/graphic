#include <iostream>
#include <cstring>
#include <cmath>
#include <stdlib.h>
#include "sphere.h"

using namespace std;

Sphere::Sphere(){
	this->center = Point();
	this->radius = Direction();
	this->coordinates = CoordinateSystem();
	this->color = RGB();
}

//A LO MEJOR EL COORDINATE SYSTEM ESTA MAL CALCULADO POR LA CIUDAD DE REFERENCIA
Sphere::Sphere(Point center, Direction radius, RGB color){
	this->center = center;
	this->color = color;
	this->radius = radius;
	Direction k = radius;
	k.normalize(); 
	Point referencePoint = center + new Direction(radius.modulus(), 0, 0); 
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

void Sphere::setColor(RGB color){
	this->color = color;
}

void Sphere::setRadius(Direction radius){
	this->radius = radius;
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

Direction Sphere::getRadius(){
	return this->radius;
}

RGB getColor(){
	return this->color;
}

CoordinateSystem Sphere::getCoordinates(){
	return this->coordinates;
}

string Sphere::showAsString(){
	string s = "SPHERE:\n Center: " + this->center.showAsString()
			+ "\nradius: " + this->radius.showAsString()
			+ "\n Coordinates:\n" + this->coordinates.showAsString() 
			+ "\n Color:\n" + this->color.showAsString();
	return s;
}

void Sphere::show(){
	cout << this->showAsString() << endl;
}

Sphere Sphere::operator=(Sphere s){
	this->center = s.getCenter();
	this->referencePoint = s.getReferencePoint();
	this->radius = s.getradius();
	this->coordinates = s.getCoordinates();
	return *this;
}
