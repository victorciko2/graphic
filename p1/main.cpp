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
	cout << "Creating first planet... " << endl;
	float x, y, z;
	cout << "Insert center of planet (UCS): " << flush;
	cin >> x >> y >> z;
	Point c1(x,y,z);
	cout << "Insert point reference (UCS) :" << flush;
	cin >> x >> y >> z;
	Point rp1(x,y,z);
	cout << "Insert axis(UCS): " << flush;
	cin >> x >> y >> z;
	Direction axis1(x,y,z);
	Sphere s1(c1, rp1, axis1);
	cout << "Creating planetary station for previous planet..." << endl;
	cout << "Insert inclination and azimuth(degrees): " << flush;
	cin >> x >> y;
	PlanetaryStation p1(s1, x*(M_PI/180), y*(M_PI/180));
	cout << "Creating second planet... " << endl;
	cout << "Insert center of planet (UCS): " << flush;
	cin >> x >> y >> z;
	Point c2(x,y,z);
	cout << "Insert point reference (UCS) :" << flush;
	cin >> x >> y >> z;
	Point rp2(x,y,z);
	cout << "Insert axis(UCS): " << flush;
	cin >> x >> y >> z;
	Direction axis2(x,y,z);
	Sphere s2(c2, rp2, axis2);
	cout << "Creating planetary station for previous planet..." << endl;
	cout << "Insert inclination and azimuth(degrees): " << flush;
	cin >> x >> y;
	PlanetaryStation p2(s2, x*(M_PI/180), y*(M_PI/180));
	cout << endl << "Calculating path..." << endl;
	Direction d = p2.getPosition() - p1.getPosition();
	cout << "Direction in UCS: " << d.showAsString() << endl;
	Direction d1 = p1.getCoordinates() * d;
	cout << "Direction in launching PS coordinates: " << d1.showAsString() << endl;
	p1.getCoordinates().show();
	Direction d2 = p2.getCoordinates() * d;
	cout << "Direction in receiving PS coordinates: " << d2.showAsString() << endl;
	p2.getCoordinates().show();
	bool res = launchingCollision(p1, p2);
	res = receiverCollision(p1, p2);
	return 0;
}