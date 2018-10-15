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

	void setSphere(Sphere sphere);

	void setInclination(float inclination);

	void setAzimuth(float azimuth);

	void setCoordinates(CoordinateSystem c);
	
	Sphere getSphere();

	float getInclination();

	float getAzimuth();

	Point getPosition();

	CoordinateSystem getCoordinates();

	string showAsString();

	void show();

};

bool launchingCollision(PlanetaryStation p1, PlanetaryStation p2);

bool receiverCollision(PlanetaryStation p1, PlanetaryStation p2);

bool solveQuadratic(float a, float b, float c, float& t0, float& t1);

#endif

