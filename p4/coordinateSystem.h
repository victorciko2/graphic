#ifndef COORDINATESYSTEM_H_
#define COORDINATESYSTEM_H_
#include <iostream>
#include <cstring>
#include <array>
#include "direction.h"
#include "point.h"

using namespace std;

using Matrix = std::array<std::array<float, 4>, 4>;
using Vector = std::array<float, 4>;

class Direction;

class Point;

class CoordinateSystem{
private:
	Matrix M;
public:
	CoordinateSystem();

	CoordinateSystem(Direction x, Direction y, Direction z, Point o);

	Matrix& getM();

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

	Point operator*(Point p);

	Direction operator*(Direction d);

	array <array<float, 4>, 4> invert(bool& hasInverse);

};

Vector operator*(const Matrix &a, const Vector &b);

#endif