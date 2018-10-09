#ifndef POINT_H
#define POINT_H
#include <iostream>
#include <cstring>
#include "direction.h"
#include "coordinateSystem.h"

using namespace std;

class Direction;

class CoordinateSystem;

class Point{
private:
	float x, y, z;	

public:
	Point();

	Point(float x, float y, float z);

	void setX(float x);

	void setY(float y);

	void setZ(float z);

	float getX();

	float getY();

	float getZ();

	void show();

	Point operator+(Direction d);

	Direction operator-(Point p2);

	Point operator=(Point p);

	Point operator*(CoordinateSystem c);

	string showAsString();
};

#endif