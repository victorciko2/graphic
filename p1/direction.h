#ifndef DIRECTION_H
#define DIRECTION_H
#include <iostream>
#include <cstring>
#include "point.h"

using namespace std;

class Point;

class Direction{
private:
	float x, y, z;	

public:
	Direction();

	Direction(float x, float y, float z);

	void setX(float x);

	void setY(float y);

	void setZ(float z);

	float getX();

	float getY();

	float getZ();

	float modulus();

	void show();

	void normalize();

	string showAsString();

	Point operator+(Point p);

	Direction operator+(Direction d2);

	Direction operator-(Direction d2);

	Direction operator*(float s);

	Direction operator/(float s);

	Direction operator=(Direction d);

	bool operator==(Direction d);
};

float dotProduct(Direction d1, Direction d2);

Direction crossProduct(Direction d1, Direction d2);

#endif
