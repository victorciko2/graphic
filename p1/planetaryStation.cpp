#include <iostream>
#include <cmath>
#include "planetaryStation.h"

using namespace std;

PlanetaryStation::PlanetaryStation(){
	this->sphere = Sphere();
	this->inclination = 0;
	this->azimuth = 0;
	Direction radius = sphere.getReferencePoint() - sphere.getCenter();
	this->position = Point(sphere.getCenter().getX() +  radius.modulus() * (sin(inclination) * sin(azimuth)),
			sphere.getCenter().getY() + radius.modulus() * (sin(inclination) * cos(azimuth)),
			sphere.getCenter().getZ() + radius.modulus() * cos(inclination));
}

PlanetaryStation::PlanetaryStation(Sphere sphere, float inclination, float azimuth){
	this->sphere = sphere;
	this->inclination = inclination;
	this->azimuth = azimuth;
	Direction radius = sphere.getReferencePoint() - sphere.getCenter();
	this->position = Point(sphere.getCenter().getX() +  radius.modulus() * (sin(inclination) * cos(azimuth)),
			sphere.getCenter().getY() + radius.modulus() * (sin(inclination) * sin(azimuth)),
			sphere.getCenter().getZ() + radius.modulus() * cos(inclination));
}	

void PlanetaryStation::setS(Sphere sphere){
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

Sphere PlanetaryStation::getS(){
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