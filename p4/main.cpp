#include <iostream>
#include <vector>
#include <fstream>	
#include <limits>
#include <string>
#include <chrono>
#include <thread>
#include "shape.h"
#include "camera.h"

using namespace std;

const int colorRange = 255;

int main(int argc, char* argv[]){//may the normal vector point outwards
	if(argc != 5){
		cout << "ERROR: Use " << argv[0] << " numPaths width height fileName" << endl; 
		return -1;
	}
	int	numPaths = atoi(argv[1]);
	int pixelX = atoi(argv[2]);
	int pixelY = atoi(argv[3]);
	string f = argv[4]; 
	Scene scene;
	Shape *shape = nullptr;
	// CORNELL BOX
	shape = new Plane(Direction(1, 0, 0), Point(-10, 0, 10), new BRDF(0.7, 0, 100, RGB((float)255/255, 0, 0))); //LEFT
	scene.add(shape);
	shape = new Plane(Direction(-1, 0, 0), Point(10, 0, -10), new BRDF(0.7, 0, 100, RGB(0, (float)255/255, 0))); //RIGHT
	scene.add(shape);
	shape = new Plane(Direction(0, 1, 0), Point(0,-10, 10), new BRDF(0.7, 0, 100, RGB((float)206/255, (float)206/255, (float)206/255))); //DOWN
	scene.add(shape);
	shape = new Plane(Direction(0, -1, 0), Point(0, 10, 10), new BRDF(0.7, 0, 100, RGB((float)255/255, (float)0/255, (float)140/255))); //UP
	scene.add(shape);
	shape = new Plane(Direction(0, 0, -1), Point(0, 00, 20), new BRDF(0.7, 0, 100, RGB((float)191/255, (float)191/255, (float)0/255))); //BACK
	scene.add(shape);

	// Pruebas sin mas
	Material* m = new BRDF(0, 0.7, 1, RGB(0, 0, (float)255/255));	
	m = new BRDF(0.7, 0, 100, RGB((float)255/255, 0, (float)255/255));
	Material* r = new Reflective();
	shape = new Parallelepiped(new Triangle(Point(7, -5, 10), Point(7, -10, 15), Point(7, -5, 20), r),
					new Triangle(Point(7, -5, 10), Point(7, 0, 15), Point(7, -5, 20), r), 12, r);
	scene.add(shape);
	shape = new Sphere(Point(-5,5,10),2.5, new Reflective());
	//shape = new Plane(Direction(0,0,1), Point(0,0,10), new Refractive(1.4, false));
	//shape = new Triangle(Point(-5,-5,10),Point(5,-5,10), Point(0,5,10), new Refractive(2));
	//scene.add(shape);
	shape = new Sphere(Point(5,0,10),2.5, new Refractive(4, false));
	//scene.add(shape);
	shape = new Cylinder(Disk(Direction(0, -1, 0), Point(0, -6, 12), 2, m), Disk(Direction(0, -1, 0), Point(0, 4, 12), 2, m), new Refractive(2, true));
	shape = new Cylinder(Disk(Direction(0, -1, 0), Point(0, -10, 10), 1, m), Disk(Direction(0, -1, 0), Point(0, 4, 10), 1, m), m);
	//scene.add(shape);
	PointLight* pl = new PointLight(Point(0, 6, 10), new Light(250, RGB(1,1,1)));
	scene.add(pl);
	
	// MAIN CODE
	Camera camera = Camera(Point(0, 0, 0), Direction(0, 0, 10), Direction(10, 0, 0), pixelX, pixelY);	camera.setL(camera.getL() * -1);
	ofstream o(f + ".ppm");
	o << "P3" << endl;
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
  	uniform_real_distribution<float> distributionX = uniform_real_distribution<float>(0, 1);
  	uniform_real_distribution<float> distributionY = uniform_real_distribution<float>(0, planeVectorU.modulus());
	for(int i = 0; i < camera.getY(); i++){
		for(int j = 0; j < camera.getX(); j++){
			RGB mean = RGB(0, 0, 0);
			for(int k = 0; k < numPaths; k++){
				rndX = distributionX(gen); rndY = distributionY(gen);
				Point pointRnd = Point(aux[0] + distributionX(gen)*planeVectorL.modulus(), 
										aux[1] + distributionX(gen)*planeVectorU.modulus(), aux[2]);
				Direction auxPi=Direction(distributionX(gen)*2*camera.getL().modulus()/camera.getX(), distributionX(gen)*2*camera.getU().modulus()/camera.getY(), 0);
				Point aux;
				d = (pointRnd +  (planeVectorL * j) + (planeVectorU * i))  - camera.getOrigin();
				d.normalize();
				//d.show();
				Ray r(d, camera.getOrigin());
				int depth = 0;
				RGB result = r.tracePath(scene, depth);
				mean = RGB(mean[0] + result[0], mean[1] + result[1], mean[2] + result[2]);
			}
			mean = RGB((mean[0] / numPaths) * colorRange, (mean[1] / numPaths) * colorRange, (mean[2] / numPaths) * colorRange);
			if(mean[0] > colorRange || mean[0] < 0) mean = RGB(colorRange, mean[1], mean[2]); 
			if(mean[1] > colorRange || mean[1] < 0) mean = RGB(mean[0], colorRange, mean[2]);
			if(mean[2] > colorRange || mean[2] < 0) mean = RGB(mean[0], mean[1], colorRange);
			o << (int) mean[0] << " " << (int) mean[1] << " " << (int) mean[2] << " ";
		}
		o << endl;
		cout << i << endl;
	}
	o.close();
	

	return 0;
}
