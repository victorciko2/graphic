#ifndef POINT_H
#define POINT_H
#include <iostream>
#include "direction.h"

using namespace std;

class Direction;

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

};

#endif