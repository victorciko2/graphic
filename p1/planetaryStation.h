#ifndef PLANETARYSTATION_H
#define PLANETARYSTATION_H
#include <iostream>
#include "sphere.h"
#include "point.h"
#include "direction.h"
#include "coordinateSystem.h"

using namespace std;

class Sphere;

class CoordinateSystem;

class PlanetaryStation{

private:
	Sphere sphere;
	float inclination, azimuth;
	Point position;
	CoordinateSystem coordinates;

public:
	PlanetaryStation();
	
	PlanetaryStation(Sphere sphere, float inclination, float azimuth);

	void setS(Sphere sphere);

	void setInclination(float inclination);

	void setAzimuth(float azimuth);

	Sphere getS();

	float getInclination();

	float getAzimuth();

	Point getPosition();

	string showAsString();

	void show();

};

#endif

