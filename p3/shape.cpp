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

float Shape::collision(Direction d, Point o, bool& collision){
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
	this->radius = -1; //poner un float
	this->color = RGB();
}

//A LO MEJOR EL COORDINATE SYSTEM ESTA MAL CALCULADO POR LA CIUDAD DE REFERENCIA
Sphere::Sphere(Point center, float radius, RGB color){
	this->center = center;
	this->color = color;
	this->radius = radius;
}

void Sphere::setCenter(Point center){
	this->center = center; 
}

void Sphere::setRadius(float radius){
	this->radius = radius;
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

float Sphere::getRadius(){
	return this->radius;
}

float Sphere::collision(Direction d, Point o, bool& collision){  // cambiar radio por float
	d.normalize();
	float a = d * d;
	float b = 2 * (d * (o - this->center));
	float c = (o - this->center)*(o - this->center) - radius * radius;
	float t0, t1;
	collision = solveQuadratic(a, b, c, t0, t1);
	if(collision){
		if(t0 > 0){
			return t0;
		}
		else{
			return -1;
		}
	}
	else{
		return -1;
	}
}

string Sphere::showAsString(){
	string s = "SPHERE:\n Center: " + this->center.showAsString()
			+ "\nradius: " + to_string(this->radius)
			+ "\n Color:\n" + this->color.showAsString();
	return s;
}

void Sphere::show(){
	cout << this->showAsString() << endl;
}
	
Sphere Sphere::operator=(Sphere s){
	this->center = s.getCenter();
	this->radius = s.getRadius();
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

float Plane::collision(Direction d, Point o, bool& collision){
	float t;
	d.normalize();
	Direction n = this->normal;
	n.normalize();
	float aux = d * n;
	if(abs(aux) > 0.00000001f){
		Direction l = this->o - o;
		t = (l * n) / aux;
		collision = true;
		return t;
	}
	collision = false;
	return -1;
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

Triangle::Triangle() {
	this->a = Point();
	this->b = Point();
	this->c = Point();
	this->p = Plane();
	this->color = RGB();
}

Triangle::Triangle(Point a, Point b, Point c, Plane p, RGB color) {
	this->a = a;
	this->b = b;
	this->c = c;
	this->p = p;
	this->color = color;
}


void Triangle::setA(Point a){
	this->a = a;
}

void Triangle::setB(Point b){
	this->b = b;
}

void Triangle::setC(Point c){
	this->c = c;
}

void Triangle::setP(Plane p){
	this->p = p;
}

void Triangle::setColor(RGB color){
	this->color = color;
}



RGB Triangle::getColor(){
	return this->color;
}

Point Triangle::getA(){
	return this->a;
}

Point Triangle::getB(){
	return this->b;
}

Point Triangle::getC(){
	return this->c;
}

Plane Triangle::getP(){
	return this->p;
}

float Triangle::collision(Direction d, Point o, bool& collision){
	float dist=this->p.collision(d,o,collision);
	if(dist > 0){
		//cout << " colisiona con el plano" << endl;
		Direction edge0 = this->b - this->a; 
		Direction edge1 = this->c - this->b; 
		Direction edge2 = this->a - this->c; 
		Direction C0 = (o+d*dist) - this->a; 
		Direction C1 = (o+d*dist) - this->b; 
		Direction C2 = (o+d*dist) - this->c; 
		if (p.getNormal()*(edge0^C0) > 0 && p.getNormal()*(edge1^C1) > 0 && p.getNormal()*(edge2^C2) > 0){
			collision = true;
			return 1;
		}
		else{
			collision = false;
			return -1;
		}
	}
	else{
		collision = false;
		return -1;
	}
	
}

string Triangle::showAsString(){
	string s = "TRIANGLE:\n Point A: " + this->a.showAsString()
			+ "\nPoint B: " + this->b.showAsString()
			+ "\nPoint C: " + this->c.showAsString();
	return s;
}

void Triangle::show(){
	cout << this->showAsString() << endl;
}

Triangle Triangle::operator=(Triangle t){
	this->a = t.getA();
	this->b = t.getB();
	this->c = t.getC();
	this->color = t.getColor();	
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