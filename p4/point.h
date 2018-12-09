<<<<<<< HEAD
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

	bool operator==(Point p);

	float operator[](int i);
	
	string showAsString();
};

=======
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

	bool operator==(Point p);

	float operator[](int i);
	
	string showAsString();
};

>>>>>>> ad5c43ce0219b035b540c93d808a284bf5abf18b
#endif