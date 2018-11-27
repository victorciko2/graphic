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

Disk::Disk(){
	this->normal = Direction();
	this->o = Point();
	this->radius = -1;
	this->color = RGB();
}

Disk::Disk(Direction normal, Point o, float radius, RGB color){
	this->normal = normal;
	this->radius = radius;
	this->o = o;
	this->color = color;
}

Point Disk::getO(){
	return this->o;
}

float Disk::getRadius(){
	return this->radius;
}

Direction Disk::getNormal(){
	return this->normal;
}

RGB Disk::getColor(){
	return this->color;
}

void Disk::setO(Point o){
	this->o = o;
}

void Disk::setRadius(float radius){
	this->radius = radius;
}

void Disk::setColor(RGB color){
	this->color = color;
}

void Disk::setNormal(Direction normal){
	this->normal = normal;
}

float Disk::collision(Direction d, Point o, bool& collision){
	d.normalize();
	Plane aux = Plane(this->normal, this->o, this->color);
	float dist = aux.collision(d, o, collision);
	if(collision && dist >= 0){
		Point intersection = o + d * dist;
		if(abs((intersection - this->o).modulus()) <= this->radius){
			collision = true;
			return dist;
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

InfiniteCylinder::InfiniteCylinder(){
	this->v = Direction();
	this->p = Point();
	this->radius = -1;
	this->color = RGB();
}

InfiniteCylinder::InfiniteCylinder(Direction v, Point p, float radius, RGB color){
	this->v = v;
	this->v.normalize();
	this->p = p;
	this->radius = radius;
	this->color = color;
}

Direction InfiniteCylinder::getDirection(){
	return this->v;
}

Point InfiniteCylinder::getPoint(){
	return this->p;
}

float InfiniteCylinder::getRadius(){
	return this->radius;
}

RGB InfiniteCylinder::getColor(){
	return this->color;
}

void InfiniteCylinder::setDirection(Direction v){
	this->v = v;
	this->v.normalize();
}

void InfiniteCylinder::setPoint(Point p){
	this->p = p;
}

void InfiniteCylinder::setRadius(float radius){
	this->radius = radius;
}

void InfiniteCylinder::setColor(RGB color){
	this->color = color;
}

float InfiniteCylinder::collision(Direction d, Point o, bool& collision){
	float t0, t1,t;
	d.normalize();
	collision = true;
	Direction iP = o - this->p;
    float a = (d - (this->v * (d * this->v))) * (d - (this->v * (d * this->v))); 
    float b = 2 * ((d - this->v * (d * this->v)) * (iP - (this->v * (iP * this->v))));
    float c = (iP - (this->v * (iP * this->v))) * (iP - (this->v * (iP * this->v))) 
    			- this->radius * this->radius; 
    if (!solveQuadratic(a, b, c, t0, t1)){
    	collision = false;
    	return -1;
    }
    if (t0 > t1) std::swap(t0, t1);
    if (t0 < 0) { 
        t0 = t1; 
        if (t0 < 0){
        	collision = false;
        	return -1;
        }  
    }
    t = t0; 
	return t;
}

Cylinder::Cylinder(){
	this->v = Direction();
	this->p = Point();
	this->radius = -1;
	this->color = RGB();
	this->sup = Plane();
	this->inf = Plane();
}

Cylinder::Cylinder(Plane inf, float h, float radius, Direction v, Point p, RGB color){
	this->v = inf.getNormal();
	this->p = inf.getO();
	this->radius = radius;
	this->color = color;
	this->inf = inf;
	sup = Plane(inf.getNormal() * -1, inf.getO() + inf.getNormal() * h,
			inf.getColor());
}

Cylinder::Cylinder(Plane inf, Plane sup, float radius, Direction v, Point p, RGB color){
	this->v = inf.getNormal();
	this->v.normalize();
	this->p = inf.getO();
	this->radius = radius;
	this->color = color;
	this->inf = inf;
	this->sup = sup; 
}

Cylinder::Cylinder(Disk bot, Disk top, RGB color){
	this->bot = bot;
	this->top = top;
	this->v = bot.getNormal();
	this->v.normalize();
	this->p = bot.getO();
	this->color = color;
	this->radius = top.getRadius();
} 

Direction Cylinder::getDirection(){
	return this->v;
}

Point Cylinder::getPoint(){
	return this->p;
}

float Cylinder::getRadius(){
	return this->radius;
}

RGB Cylinder::getColor(){
	return this->color;
}

Plane Cylinder::getSup(){
	return this->sup;
}

Plane Cylinder::getInf(){
	return this->inf;
}

void Cylinder::setSup(Plane sup){
	this->sup = sup;
}

void Cylinder::setInf(Plane inf){
	this->inf = inf;
}

void Cylinder::setDirection(Direction v){
	this->v = v;
	this->v.normalize();
}

void Cylinder::setPoint(Point p){
	this->p = p;
}

void Cylinder::setRadius(float radius){
	this->radius = radius;
}

void Cylinder::setColor(RGB color){
	this->color = color;
}

float Cylinder::collision(Direction d, Point o, bool& collision){
	float dist, distCyl, distTop, distBot;
	d.normalize();	
	bool colCyl, colTop, colBot;
	InfiniteCylinder aux = InfiniteCylinder(this->v, this->p, this->radius, this->color);
	distCyl = aux.collision(d, o, colCyl);
	distTop = this->top.collision(d, o, colTop);
	distBot = this->bot.collision(d, o, colBot);
	if(!colCyl && !colTop && !colBot){
		collision = false;
		return -1;
	}
	else if(!colCyl && !colTop && colBot){
		collision = false;
		return -1;
	}
	else if(!colCyl && colTop && !colBot){
		collision = false;
		return -1;
	}
	else if(!colCyl && colTop && colBot){
		if(distTop < distBot){
			collision = true;
			return distTop;
		}
		else if(distBot <= distTop){
			collision = true;
			return distBot;
		}
		else{
			collision = false;
			return -1;
		}
	}
	else if(colCyl && !colTop && !colBot){
		float dist2Bot = ((o + d * distCyl) - bot.getO()).modulus();
		float dist2Top = ((o + d * distCyl) - top.getO()).modulus();
		if(dist2Bot <= dist2Top){
			if(sqrt(dist2Top * dist2Top - this->radius * this->radius) <= (this->top.getO() - this->bot.getO()).modulus()){
				collision = true;
				return distCyl;
			}
			else{
				collision = false;
				return -1;
			}
		}
		else{
			if(sqrt(dist2Bot * dist2Bot - this->radius * this->radius) <= (this->top.getO() - this->bot.getO()).modulus()){
				collision = true;
				return distCyl;
			}
			else{
				collision = false;
				return -1;
			}
		}
	}
	else if(colCyl && !colTop && colBot){
		if(distBot <= distCyl){
			collision = true;
			return distBot;
		}
		else{
			float dist2Top = ((o + d * distCyl) - top.getO()).modulus();
			if(sqrt(dist2Top * dist2Top - this->radius * this->radius) <= (this->top.getO() - this->bot.getO()).modulus()){
				collision = true; 
				return distCyl;
			}
			else{
				collision = true;
				return distBot;
			}
		}
	}
	else if(colCyl && colTop && !colBot){
		float dist2Bot = ((o + d * distCyl) - bot.getO()).modulus();
		if(sqrt(dist2Bot * dist2Bot - this->radius * this->radius) <= (this->top.getO() - this->bot.getO()).modulus()){
			collision = true;
			if(distTop < distCyl){
				return distTop;
			}
			else{
				return distCyl;
			}
		}
		else{
			collision = true;
			return distTop;
		}
	}
	else if(colCyl && colTop && colBot){
		if(distTop < distBot){
			collision = true;
			return distTop;
		}
		else if(distBot <= distTop){
			collision = true;
			return distBot;
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

Triangle::Triangle() {
	this->a = Point();
	this->b = Point();
	this->c = Point();
	this->p = Plane();
	this->color = RGB();
}

Triangle::Triangle(Point a, Point b, Point c, RGB color) {
	this->a = a;
	this->b = b;
	this->c = c;
	Direction ab = a - b;
	Direction ac = a - c;
	Direction normal = ab ^ ac;
	normal.normalize();
	this->p = Plane(normal, this->a, this->color);
	this->color = color;
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
			return dist;
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