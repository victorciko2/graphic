#ifndef SPHERE_H_
#define SPHERE_H_
#include <iostream>
#include <cstring>
#include "point.h"
#include "direction.h"
#include "coordinateSystem.h"

class Point;

class Direction;

class CoordinateSystem;

class Sphere{
private:
	Point center, referencePoint;
	Direction axis;
	CoordinateSystem coordinates;
public:
	Sphere();

	Sphere(Point center, Point referencePoint, Direction axis, CoordinateSystem coordinates);

	void setCenter(Point center);

	void setReferencePoint(Point referencePoint);

	void setAxis(Direction axis);

	void setCoordinates(CoordinateSystem coordinates);

	Point getCenter();

	Point getReferencePoint();

	Direction getAxis();

	CoordinateSystem getCoordinates();

	string showAsString();
	
	void show();

	Sphere operator=(Sphere s);
};

#endif
