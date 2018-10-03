#include <iostream>
#include <cmath>
#include "planetaryStation.h"

using namespace std;

PlanetaryStation::PlanetaryStation(){
	this->sphere = Sphere();
	this->inclination = 0;
	this->azimuth = 0;
	Direction radius = sphere.getReferencePoint() - sphere.getCenter();
	/*this->position = Point(sphere.getCenter().getX() +  radius.getX() * sin(inclination) * sin(azimuth),
			sphere.getCenter().getY() + radius.getY() * sin(inclination) * cos(azimuth),
			sphere.getCenter().getZ() + radius.getZ() * cos(inclination));
	*/
}

PlanetaryStation::PlanetaryStation(Sphere sphere, float inclination, float azimuth){
	this->sphere = sphere;
	this->inclination = inclination;
	this->azimuth = azimuth;
	Direction radius = sphere.getReferencePoint() - sphere.getCenter();
	this->position = Point(sphere.getCenter().getX() +  radius.getX() * sin(inclination) * sin(azimuth),
			sphere.getCenter().getY() + radius.getY() * sin(inclination) * cos(azimuth),
			sphere.getCenter().getZ() + radius.getZ() * cos(inclination));
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

string PlanetaryStation::showAsString(){
	string s = "PLANETARYSTATION\n { Sphere: " + this->sphere.showAsString() + ", inclination: "
		+ to_string(this->inclination) + ", azimuth: " 
		+ to_string(this->azimuth) + "\nPosistion: "
		+ position.showAsString() + "} ";
	return s;
}

void PlanetaryStation::show(){
	cout << this->showAsString() << flush;
}