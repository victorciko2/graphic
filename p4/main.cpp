#include <iostream>
#include <vector>
#include <fstream>	
#include <limits>
#include "shape.h"
#include "camera.h"

using namespace std;

int main(){
	Scene scene;
	Shape *shape = nullptr;
	shape = new Plane(Direction(1, 0, 0), Point(-20, 0, 0), BRDF(0.7, 0, 100, RGB(255, 0, 0))); //LEFT
	cout << shape << endl;
	scene.add(shape);
	shape = new Plane(Direction(1, 0, 0), Point(20, 0, 0), BRDF(0.7, 0, 100, RGB(0, 255, 0))); //RIGHT
	scene.add(shape);
	cout << shape << endl;
	shape = new Plane(Direction(0, 1, 0), Point(0, -20, 0), BRDF(0.7, 0, 100, RGB(230, 230, 230))); //DOWN
	scene.add(shape);
	cout << shape << endl;
	shape = new Plane(Direction(0, 1, 0), Point(0, 20, 0), BRDF(0.7, 0, 100, RGB(140, 140, 140))); //UP
	scene.add(shape);
	cout << shape << endl;
	shape = new Plane(Direction(0, 0, 1), Point(0, 0, 40), BRDF(0.7, 0, 100, RGB(191, 191, 191))); //BACK
	cout << shape << endl;
	scene.add(shape);
	PointLight* pl = new PointLight(Point(16, 18, 36), Light());
	scene.add(pl);
	cout << shape << endl;
	Material m = new BRDF(0.7, 0, 100, RGB(252, 123, 220));
	shape = new Sphere(Point(5, -13.5, 35), 7, m); 
	scene.add(shape);
	shape->show();
 /*//EL MARAVILLOSO PENE
	shape = new Sphere(Point(5, -13.5, 35), 7, RGB(252, 123, 220)); //RIGHT BALL
	
	shape = new Sphere(Point(-5, -13.5, 35), 7, RGB(252, 123, 220)); //LEFT BALL
	
	shape = new Cylinder(Disk(Direction(0, 1, 0), Point(0, -13, 35), 6, RGB(255, 255, 255)), 
				Disk(Direction(0,1,0), Point(0, 15, 35), 6, RGB(255,255,255)), RGB(214, 104, 187));
	
	shape = new Sphere(Point(0, 15, 35), 6, RGB(173, 83, 151)); //TOP
	*/
	
	Camera camera = Camera(Point(0, 0, 0), Direction(0, 0, 10), Direction(10, 0, 0), 480, 480);	camera.setL(camera.getL() * -1);
	ofstream o("prueba.ppm");
	o << "P3" << endl;
	o << camera.getX() << " " << camera.getY() << endl;
	o << "255" << endl;
	Direction d;
	float minDist = numeric_limits<float>::max();
	int index = -1;
	bool collision;
	float currentDist;
	Direction planeVectorL = camera.getL() * -2 / camera.getX();
	Direction planeVectorU = camera.getU() * -2 / camera.getY();
	Point aux = camera.getOrigin() + camera.getL() + camera.getU() + camera.getF() + planeVectorL / 2 + planeVectorU / 2;
	//camera.show();
	for(int i = 0; i < camera.getY(); i++){
		for(int j = 0; j < camera.getX(); j++){
			//cout << "----------------------------------------------------" << endl;
			//cout << "pixel " << j << " " << i << endl;
			d = (aux + (planeVectorL * j) + (planeVectorU * i))  - camera.getOrigin();
			d.normalize();
			Ray r(d, camera.getOrigin());
			RGB color(0, 0, 0);
			int depth = 0;
			RGB color2 = r.tracePath(scene, depth);
			o << color2[0] << " " << color2[1] << " " << color2[2] << " ";

			//cout << "----------------------------------------------------" << endl;
		}
		o << endl;
	}
	o.close();
	return 0;
}