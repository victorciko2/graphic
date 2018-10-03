#ifndef COORDINATESYSTEM_H_
#define COORDINATESYSTEM_H_
#include <iostream>
#include <cstring>
#include <array>
#include "direction.h"
#include "point.h"

using namespace std;

class Direction;

class Point;

class CoordinateSystem{
private:
	array <array<float, 4>, 4> matrix;
public:
	CoordinateSystem();

	CoordinateSystem(array <array<float, 4>, 4> matrix);

	void setMatrix(array <array<float, 4>, 4> matrix);

	void setI(Direction i);

	void setJ(Direction j);

	void setK(Direction k);

	void setO(Point o);

	Direction getI();

	Direction getJ();

	Direction getK();

	Point getO();

	array <array<float, 4>, 4> getMatrix();

	string showAsString();

	void show();

	CoordinateSystem operator=(CoordinateSystem c);

	array <array<float, 4>, 4> invert(bool& hasInverse);

	Point operator*(Point p);
};

#endif