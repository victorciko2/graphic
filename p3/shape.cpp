#include <iostream>
#include <cmath>
#include "shape.h"

using namespace std;

Shape::Shape(){
	this->color = RGB();
}

Shape::Shape(RGB color){
	this->color = color;
}

void Shape::setColor(RGB color){
	this->color = color;
}

RGB Shape::getColor(){
	return this->color;
}

float Shape::collision(Direction d, Point o){
	cout << "Failed to calculate collision" << endl;
	return -1;
}

string Shape::showAsString(){
	return "SHAPE:\n";
}

void Shape::show(){
	cout << this->showAsString() << endl;
}

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
	Point referencePoint = center + Direction(radius.modulus(), 0, 0); 
	Direction aux = referencePoint - this->center;
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

void Sphere::setRadius(Direction radius){
	this->radius = radius;
}

void Sphere::setCoordinates(CoordinateSystem coordinates){
	this->coordinates = coordinates;
}

void Sphere::setColor(RGB color){
	this->color = color;
}

RGB Sphere::getColor(){
	return this->color;
}

Point Sphere::getCenter(){
	return this->center;
}

Direction Sphere::getRadius(){
	return this->radius;
}

CoordinateSystem Sphere::getCoordinates(){
	return this->coordinates;
}

float Sphere::collision(Direction d, Point o){
	d.normalize();
	//Point o = p2.getPosition();
	//Direction radius = p2.getSphere().getAxis() / 2;
	float r = radius.modulus();
	float a = 1;
	float b = 2 * (d * (o - this->center));
	float c = (o - this->center)*(o - this->center) - r * r;
	float t0, t1;
	bool sol = solveQuadratic(a, b, c, t0, t1);
	if(sol){
		Point point1 = o + d * t0;
		Point point2 = o + d * t1;
		if(t1 > 0){
			//cout << "Collision with receiver planet" << endl;
			return t1;
		}
		else{
			//cout << "There's no collision with receiver planet" << endl;
			return -1;
		}
	}
	else{
		return -1;
	}
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
	this->radius = s.getRadius();
	this->coordinates = s.getCoordinates();
	this->color = s.getColor();	
	return *this;
}

Plane::Plane(){
	this->o = Point();
	this->normal = Direction();
	this->color = RGB();
}

Plane::Plane(Direction normal, Point o, RGB color){
	this->o = o;
	this->color = color;
	this->normal = normal;
}

void Plane::setO(Point o){
	this->o = o; 
}

void Plane::setNormal(Direction normal){
	this->normal = normal;
}

void Plane::setColor(RGB color){
	this->color = color;
}

RGB Plane::getColor(){
	return this->color;
}

Point Plane::getO(){
	return this->o;
}

Direction Plane::getNormal(){
	return this->normal;
}

float Plane::collision(Direction d, Point o){
	d.normalize();
	//Point o = p2.getPosition();
	//Direction radius = p2.getSphere().getAxis() / 2;
	float r = radius.modulus();
	float a = 1;
	float b = 2 * (d * (o - this->center));
	float c = (o - this->center)*(o - this->center) - r * r;
	float t0, t1;
	bool sol = solveQuadratic(a, b, c, t0, t1);
	if(sol){
		Point point1 = o + d * t0;
		Point point2 = o + d * t1;
		if(t1 > 0){
			//cout << "Collision with receiver planet" << endl;
			return t1;
		}
		else{
			//cout << "There's no collision with receiver planet" << endl;
			return -1;
		}
	}
	else{
		return -1;
	}
}

string Plane::showAsString(){
	string s = "PLANE:\n Point: " + this->o.showAsString()
			+ "\nnormal: " + this->normal.showAsString();
	return s;
}

void Plane::show(){
	cout << this->showAsString() << endl;
}
	
Plane Plane::operator=(Plane p){
	this->o = p.getO();
	this->normal = p.getNormal();
	this->color = p.getColor();	
	return *this;
}


bool solveQuadratic(float a, float b, float c, float& t0, float& t1){
	float discr = b * b - 4 * a * c; //discriminant
	if(discr < 0){ //doesnt have solution
		return false;
	}
	else if(discr == 0){//one solution
		t0 = -b / (2 * a);
		t1 = t0;
	}
	else{//two solutions
		t0 = (-b + sqrt(discr)) / (2 * a);
		t1 = (-b - sqrt(discr)) / (2 * a); 
	}
	if(t0 > t1){
		swap(t0, t1);
	}
	return true;
}