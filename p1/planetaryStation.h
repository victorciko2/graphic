#ifndef PLANETARYSTATION_H
#define PLANETARYSTATION_H
#include <iostream>
#include "sphere.h"
#include "point.h"

using namespace std;

class Sphere;

class PlanetaryStation{

private:
	Sphere s;
	float inclination, azimuth;

public:
	PlanetaryStation();
	PlanetaryStation(Sphere s, float inclination, float azimuth);

	void setS(Sphere s);

	void setInclination(float inclination);

	void setAzimuth(float azimuth);

	Sphere getS();

	float getInclination();

	float getAzimuth();

	void show();

};

#endif

