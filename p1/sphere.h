#ifndef SPHERE_H_
#define SPHERE_H_
#include <iostream>
#include "point.h"
#include "direction.h"

class Point;

class Direction;

class Sphere{
private:
	Point center, referencePoint;
	Direction axis;
public:
	Sphere();

	Sphere(Point center, Point referencePoint, Direction axis);

	void setCenter(Point center);

	void setReferencePoint(Point referencePoint);

	void setDirection(Direction axis);

	Point getCenter();

	Point getReferencePoint();

	Direction getAxis();

	void show();

	Sphere operator=(Sphere s);
};

#endif
