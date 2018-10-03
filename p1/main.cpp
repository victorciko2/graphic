#include <iostream>
#include <cstring>
#include <stdio.h>
#include <array>
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
	Sphere s(Point(3, 3, 3), Point(3, 3, 3) + radio, Direction(0, 0, 2));
	s.show(); 
	cout << "Producto escalar" << endl;
	float pollas = dotProduct((s.getReferencePoint() - s.getCenter()), (s.getReferencePoint() - s.getCenter())) - 1;
	cout << "numero magico " << pollas << endl; 
	PlanetaryStation p(s, M_PI/2.0, 0);
	cout << "position from sphere: " << p.getPosition().showAsString() << endl;
	
	float pollas2 = dotProduct((p.getPosition() - s.getCenter()), (p.getPosition() - s.getCenter())) - 1;
	cout << "numero magico2 " << pollas2 << endl; 
	return 0;
}