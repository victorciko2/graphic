#ifndef COORDINATESYSTEM_H_
#define COORDINATESYSTEM_H_
#include <iostream>
#include <cstring>
#include "direction.h"

using namespace std;

class Direction;

class CoordinateSystem{
private:
	Direction i, j, k;
public:
	CoordinateSystem();

	CoordinateSystem(Direction i, Direction j, Direction k);

	void setI(Direction i);

	void setJ(Direction j);

	void setK(Direction k);

	Direction getI();

	Direction getJ();

	Direction getK();

	string showAsString();

	void show();

	CoordinateSystem operator=(CoordinateSystem c);

};

#endif