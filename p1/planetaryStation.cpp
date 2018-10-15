#include <iostream>
#include <cmath>
#include "planetaryStation.h"

using namespace std;

PlanetaryStation::PlanetaryStation(){
	this->sphere = Sphere();
	this->inclination = 0;
	this->azimuth = 0;
	Direction radius = sphere.getReferencePoint() - sphere.getCenter();
	this->position = Point(sphere.getCenter().getX() +  radius.modulus() * (sin(inclination) * cos(azimuth)),
			sphere.getCenter().getY() + radius.modulus() * (sin(inclination) * sin(azimuth)),
			sphere.getCenter().getZ() + radius.modulus() * cos(inclination));
}

PlanetaryStation::PlanetaryStation(Sphere sphere, float inclination, float azimuth){
	this->sphere = sphere;
	this->inclination = inclination;
	this->azimuth = azimuth;
	Direction radius = sphere.getReferencePoint() - sphere.getCenter();
	this->position = Point(radius.modulus() * (sin(inclination) * cos(azimuth)),
			radius.modulus() * (sin(inclination) * sin(azimuth)),
			radius.modulus() * cos(inclination));
	this->position = this->sphere.getCoordinates() * this->position;
	Direction auxK = this->position - this->sphere.getCenter();
	auxK.normalize();
	this->coordinates.setK(auxK);
	Direction auxI = auxK ^ sphere.getCoordinates().getK();
	auxI.normalize();
	this->coordinates.setI(auxI);
	Direction auxJ = auxK ^ auxI;
	auxJ.normalize();
	this->coordinates.setJ(auxJ);
	this->coordinates.setO(this->position);
}	

void PlanetaryStation::setSphere(Sphere sphere){
	this->sphere = sphere;
}

void PlanetaryStation::setInclination(float inclination){
	this->inclination = inclination;
}

void PlanetaryStation::setAzimuth(float azimuth){
	this->azimuth = azimuth;	
}

void PlanetaryStation::setCoordinates(CoordinateSystem c){
	this->coordinates = c;
}

Sphere PlanetaryStation::getSphere(){
	return this->sphere;
}

float PlanetaryStation::getInclination(){
	return this->inclination;
}

float PlanetaryStation::getAzimuth(){
	return this->azimuth;
}

Point PlanetaryStation::getPosition(){
	return this->position;
}

CoordinateSystem PlanetaryStation::getCoordinates(){
	return this->coordinates;
}

string PlanetaryStation::showAsString(){
	string s = "PLANETARYSTATION\n { Sphere: " + this->sphere.showAsString() + ", inclination: "
		+ to_string(this->inclination) + ", azimuth: " 
		+ to_string(this->azimuth) + "\nPosistion: "
		+ this->position.showAsString() + "} ";
	return s;
}

void PlanetaryStation::show(){
	cout << this->showAsString() << flush;
}

//p1: launching, p2: receiver
bool launchingCollision(PlanetaryStation p1, PlanetaryStation p2){
	Direction d = p1.getPosition() - p2.getPosition(); //ray
	d.normalize();
	d.show();
	Point o = p1.getPosition();
	Point center = p1.getSphere().getCenter();
	Direction radius = p1.getSphere().getAxis() / 2;
	float r = radius.modulus();
	float a = 1;
	float b = 2 * (d * (o - center));
	float c = (o - center)*(o - center) - r * r;
	float t0, t1;
	bool sol = solveQuadratic(a, b, c, t0, t1);
	if(sol){
		cout << "LAUNCHING RESULTS: " << t0 << ", " << t1 << endl;
		cout << "COLLISION POINTS" << endl;
		Point point1 = p1.getPosition() + d * t0;
		Point point2 = p1.getPosition() + d * t1;
		cout << "1: " <<  point1.showAsString() << endl;
		cout << "2: " << point2.showAsString() << endl;
		if(t0 < 0){
			cout << "Collision with launching planet" << endl;
			return true;
		}
		else{
			cout << "There's no collision with launching planet" << endl;
			return false;
		}
	}
}

//p1: launching, p2: receiver
bool receiverCollision(PlanetaryStation p1, PlanetaryStation p2){
	Direction d = p2.getPosition() - p1.getPosition(); //ray
	d.normalize();
	Point o = p2.getPosition();
	Point center = p2.getSphere().getCenter();
	Direction radius = p2.getSphere().getAxis() / 2;
	float r = radius.modulus();
	float a = 1;
	float b = 2 * (d * (o - center));
	float c = (o - center)*(o - center) - r * r;
	float t0, t1;
	bool sol = solveQuadratic(a, b, c, t0, t1);
	if(sol){
		cout << "RECEIVER RESULTS: " << t0 << ", " << t1 << endl;
		cout << "COLLISION POINTS" << endl;
		Point point1 = p2.getPosition() + d * t0;
		Point point2 = p2.getPosition() + d * t1;
		cout << "1: " <<  point1.showAsString() << endl;
		cout << "2: " << point2.showAsString() << endl;
		if(t0 < 0){
			cout << "Collision with receiver planet" << endl;
			return true;
		}
		else{
			cout << "There's no collision with receiver planet" << endl;
			return false;
		}
	}
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