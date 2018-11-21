#ifndef SHAPE_H_
#define SHAPE_H_
#include <iostream>
#include <cstring>
#include "point.h"
#include "direction.h"
#include "rgb.h"

class Point;

class Direction;

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
protected:
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

class Disc : public Plane {
protected:
	float radius;
public:
	Disc();

	Disc(Direction normal, Point o, float r, RGB color);
	
	Point getO();

	float getRadius();

	Direction getNormal();

	RGB getColor();

	void setO(Point o);

	void setRadius(float radius);

	void setColor(RGB color);

	void setNormal(Direction normal);

	float collision(Direction d, Point o, bool& collision);
};

class InfiniteCylinder : public Shape{
protected:
	Direction d; // Cylinder direction
	Point p; // point origin of d
	float radius;
public:
	InfiniteCylinder();

	InfiniteCylinder(Direction d, Point p, float r, RGB color);

	Direction getDirection();

	Point getPoint();

	float getRadius();

	RGB getColor();

	void setDirection(Direction d);

	void setPoint(Point p);

	void setRadius(float radius);

	void setColor(RGB color);

	float collision(Direction d, Point o, bool& collision);
};

class Cylinder : public InfiniteCylinder{
private:
	Plane sup;
	Plane inf;
	Disc top, bot;
public:
	Cylinder();

	Cylinder(Plane inf, float h, float radius, Direction d, Point p, RGB color);

	Cylinder(Plane inf, Plane sup, float radius, Direction d, Point p, RGB color);	

	Cylinder(Disc bot, Disc top, RGB color);

	Direction getDirection();

	Point getPoint();

	float getRadius();

	RGB getColor();

	Plane getSup();

	Plane getInf();

	void setSup(Plane sup);

	void setInf(Plane inf);

	void setDirection(Direction d);

	void setPoint(Point p);

	void setRadius(float radius);

	void setColor(RGB color);

	float collision(Direction d, Point o, bool& collision);
class Triangle : public Shape{
private:
	Point a;
	Point b;
	Point c;
	Plane p;
public:
	Triangle();

	Triangle(Point a, Point b, Point c, Plane p, RGB color);

	void setA(Point a);

	void setB(Point b);

	void setC(Point c);

	void setP(Plane p);

	void setColor(RGB color);

	RGB getColor();

	Point getA();

	Point getB();

	Point getC();

	Plane getP();

	float collision(Direction d, Point o, bool& collision);

	string showAsString();

	void show();

	Triangle operator=(Triangle t);

bool solveQuadratic(float a, float b, float c, float& t0, float& t1);

#endif