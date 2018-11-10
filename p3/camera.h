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
public:
	Camera();

	Camera(Point o, CoordinateSystem c);

	Point getOrigin();

	CoordinateSystem getCoordinateSystem();

	void setOrigin(Point o);

	void setCoordinateSystem();

	string showAsString();

	void show();
};

#endif