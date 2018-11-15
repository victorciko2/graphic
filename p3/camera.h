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
	Direction l, f, u;
	int x, y;
public:
	Camera();

	Camera(Point o, CoordinateSystem c, long int x, long int y);

	Camera(Point o, Direction f, Direction l, Direction u, long int x);
	
	Point getOrigin();

	CoordinateSystem getCoordinateSystem();

	long int getX();

	long int getY();

	Direction getL();

	Direction getF();

	Direction getU();

	void setL(Direction l);

	void setF(Direction f);

	void setU(Direction u);

	void setOrigin(Point o);

	void setCoordinateSystem();

	void setX(long int x);

	void setY(long int y);

	string showAsString();

	void show();
};

#endif