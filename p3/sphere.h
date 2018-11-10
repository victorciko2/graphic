#ifndef SPHERE_H_
#define SPHERE_H_
#include <iostream>
#include <cstring>
#include "point.h"
#include "direction.h"
#include "coordinateSystem.h"
#include "rgb.h"

class Point;

class Direction;

class CoordinateSystem;

class Sphere{
private:
	Point center;
	Direction radius;
	CoordinateSystem coordinates;
	RGB color;
public:
	Sphere();

	Sphere(Point center, Direction radius, RGB color);

	void setCenter(Point center);

	void setRadius(Direction radius);

	void setColor(RGB color);

	void setCoordinates(CoordinateSystem coordinates);

	Point getCenter();

	RGB getColor();

	Direction getRadius();

	CoordinateSystem getCoordinates();

	string showAsString();
	
	void show();

	Sphere operator=(Sphere s);
};

#endif
