#include <iostream>
#include <cstring>
#include <stdio.h>
#include <array>
#include <cmath>
#include "point.h"
#include "direction.h"
#include "sphere.h"
#include "planetaryStation.h"
#include "coordinateSystem.h"

#define M_PI           3.14159265358979323846  /* pi */

using namespace std;

int main(){
	Direction radio(-5,8,4);
	radio.normalize();
	Sphere s(Point(3, 3, 3), Point(2, 3, 3), Direction(0, 0, 2));
	s.show(); 
	cout << "Producto escalar" << endl;
	PlanetaryStation p(s, M_PI/2.0, M_PI);
	cout << "position from sphere: " << p.getPosition().showAsString() << endl;
	Point p1(1,2,3); 
	Direction d1(1,2,3);
	bool basura;
	cout << "matrix " << s.getCoordinates().showAsString() << endl;
	array <array<float,4>,4> matrixAux = s.getCoordinates().invert(basura);
	CoordinateSystem c2;
	c2.setMatrix(matrixAux);
	cout << "invert " << c2.showAsString() << endl; 
	Point p2 = p1 * s.getCoordinates();
	cout << "point * matrix = " << p2.showAsString() << endl;
	Point p3 = s.getCoordinates() * p1;
	cout << "matrix * point = " << p3.showAsString() << endl;
	Direction d2 = d1 * s.getCoordinates();
	cout << "direction * matrix " << d2.showAsString() << endl;
	Direction d3 = s.getCoordinates() * d1;
	cout << "matrix * direction " << d3.showAsString() << endl;
	return 0;
}