#ifndef CAMERA_H_
#define CAMERA_H_
#include <iostream>
#include <cstring>
#include "point.h"
#include "direction.h"
#include "coordinateSystem.h"

class Camera{
private:
	Point o;
	CoordinateSystem c; //i = l, j = f, k = u
	long int x, y;
public:
	Camera();

	Camera(Point o, CoordinateSystem c, long int x, long int y);

	Point getOrigin();

	CoordinateSystem getCoordinateSystem();

	long int getX();

	long int getY();

	void setOrigin(Point o);

	void setCoordinateSystem();

	void setX(long int x);

	void setY(long int y);

	string showAsString();

	void show();
};

#endif