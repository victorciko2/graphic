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
	Sphere s1(Point(0, 0, 0), Point(-1, 0, 0), Direction(0, 0, 2));
	//s1.show(); 
	cout << endl << "--------------------------------" << endl;
	PlanetaryStation p1(s1, M_PI/2.0, 0);
	p1.show();
	cout << endl << "--------------------------------" << endl;
	Sphere s2(Point(-3, 0, 0), Point(-4, 0, 0), Direction(0, 0, 2));
	//s2.show(); 
	cout << endl << "--------------------------------" << endl;
	PlanetaryStation p2(s2, M_PI/2.0, 0);
	p2.show();
	cout << endl << "--------------------------------" << endl;
	bool res = launchingCollision(p1, p2);
	res = receiverCollision(p1, p2);

	return 0;
}