#include <iostream>
#include <cstring>
#include <stdio.h>
#include <array>
#include "point.h"
#include "direction.h"
#include "sphere.h"
#include "planetaryStation.h"
#include "coordinateSystem.h"
using namespace std;

int main(){
	Sphere s(Point(5, 5, 5), Point(6, 5, 5), Direction(0,0,1));
	s.show(); 
	PlanetaryStation p(s, 0, 0);
	p.show();
	CoordinateSystem c(s.getMatrix());
	return 0;
}