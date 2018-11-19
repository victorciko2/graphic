#ifndef SHAPE_H_
#define SHAPE_H_
#include <iostream>
#include <cstring>
#include "point.h"
#include "direction.h"
#include "coordinateSystem.h"
#include "rgb.h"

class Point;

class Direction;

class CoordinateSystem;

class Shape{
protected:
	RGB color;
public:
	Shape();

	Shape(RGB color);

	virtual void setColor(RGB color);

	virtual RGB getColor();

	virtual float collision(Direction d, Point o, bool& collision);

	virtual string showAsString();

	virtual void show();
};

class Sphere : public Shape{
private:
	Point center;
	float radius;
public:
	Sphere();

	Sphere(Point center, float radius, RGB color);

	void setCenter(Point center);

	void setRadius(float radius);

	void setColor(RGB color);

	RGB getColor();

	Point getCenter();

	float getRadius();

	float collision(Direction d, Point o, bool& collision);

	string showAsString();
	
	void show();

	Sphere operator=(Sphere s);
};

class Plane : public Shape{
private:
	Direction normal;
	Point o;
public:
	Plane();

	Plane(Direction normal, Point o, RGB color);

	void setO(Point o);

	void setNormal(Direction normal);

	void setColor(RGB color);

	RGB getColor();

	Point getO();

	Direction getNormal();

	float collision(Direction d, Point o, bool& collision);

	string showAsString();
	
	void show();

	Plane operator=(Plane p);
};

bool solveQuadratic(float a, float b, float c, float& t0, float& t1);

#endif