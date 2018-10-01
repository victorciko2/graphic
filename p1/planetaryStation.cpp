#include <iostream>
#include "planetaryStation.h"

using namespace std;

PlanetaryStation::PlanetaryStation(){
	this->s = Sphere();
	this->inclination = 0;
	this->azimuth = 0;
}

PlanetaryStation::PlanetaryStation(Sphere s, float inclination, float azimuth){
	this->s = s;
	this->inclination = inclination;
	this->azimuth = azimuth;
}

void PlanetaryStation::setS(Sphere s){
	this->s = s;
}

void PlanetaryStation::setInclination(float inclination){
	this->inclination = inclination;
}

void PlanetaryStation::setAzimuth(float azimuth){
	this->azimuth = azimuth;	
}

Sphere PlanetaryStation::getS(){
	return this->s;
}

float PlanetaryStation::getInclination(){
	return this->inclination;
}

float PlanetaryStation::getAzimuth(){
	return this->azimuth;
}

void PlanetaryStation::show(){
	cout << "{ planet " << flush;
	this->s.show();
	cout << ",inclination " << this->inclination << ", azimuth" << this->azimuth << "} " << flush;
}