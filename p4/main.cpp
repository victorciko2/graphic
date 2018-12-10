#include <iostream>
#include <vector>
#include <fstream>	
#include <limits>
#include <chrono>
#include <thread>
#include "shape.h"
#include "camera.h"

using namespace std;

const int colorRange = 255;
int main(){//may the normal vector point outwards
	Scene scene;
	Shape *shape = nullptr;
	shape = new Plane(Direction(-1, 0, 0), Point(-20, 0, 0), new BRDF(0.7, 0, 100, RGB(255, 0, 0))); //LEFT
	//shape = new Plane(Direction(1, 0, 0), Point(-20, 0, 0), RGB(255, 0, 0)); //LEFT
	//shape->show();
	//scene.add(shape);
	shape = new Plane(Direction(1, 0, 0), Point(20, 0, 0), new BRDF(0.7, 0, 100, RGB(0, 255, 0))); //RIGHT
	//shape = new Plane(Direction(1, 0, 0), Point(20, 0, 0), new Material(RGB(0, 255, 0)));
	//scene.add(shape);
	//shape->show();
	shape = new Plane(Direction(0, -1, 0), Point(0, -20, 0), new BRDF(0.7, 0, 100, RGB(206, 206, 206))); //DOWN
	//shape = new Plane(Direction(0, 1, 0), Point(0, -20, 0), new Material(RGB(230, 230, 230))); //DOWN
	//scene.add(shape);
	//shape->show();
	shape = new Plane(Direction(0, 1, 0), Point(0, 20, 0), new BRDF(0.7, 0, 100, RGB(140, 140, 140))); //UP
	//shape = new Plane(Direction(0, 1, 0), Point(0, 20, 0), new Material(RGB(140, 140, 140))); //UP
	//scene.add(shape);
	//shape->show();
	shape = new Plane(Direction(0, 0, 1), Point(0, 0, 40), new BRDF(0.7, 0, 100, RGB(191, 191, 191))); //BACK
	//shape = new Plane(Direction(0, 0, 1), Point(0, 0, 40), new Material(RGB(191, 191, 191))); //BACK
	//shape->show();
	//scene.add(shape);
	PointLight* pl = new PointLight(Point(0, 18, 20), new Light(2500, RGB(1,1,1)));
	scene.add(pl);
	pl = new PointLight(Point(0, 18, 20), new Light(100, RGB(1,1,1)));
	//scene.add(pl);
	pl = new PointLight(Point(0, 18, 20), new Light(100, RGB(1,1,1)));
	//scene.add(pl);
	//shape->show();
	Material* m = new BRDF(0.7, 0, 1000000000, RGB(0, 0, 255));
	shape = new Sphere(Point(-8, 0, 35), 7, m); 
	scene.add(shape);
	m = new BRDF(0.5, 0.5, 1, RGB(255, 0, 255));
	shape = new Sphere(Point(8, 0, 35), 7, m); 
	scene.add(shape);
	shape = new Triangle(Point(-18, -18,  20), Point(-18, 0, 30), Point(-10, -18, 25), m); 
	//scene.add(shape);
	shape = new Triangle(Point(18, -18,  20), Point(18, 0, 30), Point(10, -18, 25), m); 
	//scene.add(shape);
	//shape->show();
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
	int numPaths = 20;
	o << camera.getX() << " " << camera.getY() << endl;
	o << "255" << endl;
	Direction d;
	float minDist = numeric_limits<float>::max();
	int index = -1;
	bool collision;
	random_device rd;
	mt19937 gen = mt19937 (rd());
	float currentDist, rndX, rndY;
	Direction planeVectorL = camera.getL() * -2 / camera.getX();
	Direction planeVectorU = camera.getU() * -2 / camera.getY();
	Point aux = camera.getOrigin() + camera.getL() + camera.getU() + camera.getF();
  	uniform_real_distribution<float> distributionX = uniform_real_distribution<float>(0, planeVectorL.modulus());
  	uniform_real_distribution<float> distributionY = uniform_real_distribution<float>(0, planeVectorU.modulus());
	//camera.show();
	for(int i = 0; i < camera.getY(); i++){
		for(int j = 0; j < camera.getX(); j++){
			//cout << "--------------------------------------" << endl;
			//cout << "pixel = " << j << ", " << i << endl;
			RGB mean = RGB(0, 0, 0);
			for(int k = 0; k < numPaths; k++){
				rndX = distributionX(gen); rndY = distributionY(gen);
				Point pointRnd = Point(aux[0] + rndX, aux[1] + rndY, aux[2]);
				d = (pointRnd +  (planeVectorL * j) + (planeVectorU * i))  - camera.getOrigin();
				d.normalize();
				//d.show();
				Ray r(d, camera.getOrigin());
				int depth = 0;
				RGB result = r.tracePath(scene, depth);
				mean = RGB(mean[0] + result[0], mean[1] + result[1], mean[2] + result[2]);
			}
			//mean = RGB((mean[0] / numPaths) * colorRange, (mean[1] / numPaths) * colorRange, (mean[2] / numPaths) * colorRange);
			mean = RGB((mean[0] / numPaths), (mean[1] / numPaths), (mean[2] / numPaths));
			if(mean[0] > colorRange || mean[0] < 0) mean = RGB(colorRange, mean[1], mean[2]); 
			if(mean[1] > colorRange || mean[1] < 0) mean = RGB(mean[0], colorRange, mean[2]);
			if(mean[2] > colorRange || mean[2] < 0) mean = RGB(mean[0], mean[1], colorRange);
			//cout << "main : " << mean.showAsString() << endl;
			o << (int) mean[0] << " " << (int) mean[1] << " " << (int) mean[2] << " ";
			//cout << "color pal fichero = " << mean.showAsString() << endl;
			//cout << "--------------------------------------" << endl;
			//this_thread::sleep_for(std::chrono::milliseconds(5000));
		}
		o << endl;
	}
	o.close();
	return 0;
}
