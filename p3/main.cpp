#include <iostream>
#include <vector>
#include <fstream>	
#include <limits>
#include <random>
#include "shape.h"
#include "camera.h"

using namespace std;

int main(){
	vector<Shape*> list;
	Shape *shape = nullptr;
	shape = new Plane(Direction(1, 0, 0), Point(-20, 0, 0), RGB(255, 0, 0)); //LEFT
	list.push_back(shape);
	shape = new Plane(Direction(1, 0, 0), Point(20, 0, 0), RGB(0, 255, 0)); //RIGHT
	list.push_back(shape);
	shape = new Plane(Direction(0, 1, 0), Point(0, -20, 0), RGB(230, 230, 230)); //DOWN
	list.push_back(shape);
	shape = new Plane(Direction(0, 1, 0), Point(0, 20, 0), RGB(140, 140, 140)); //UP
	list.push_back(shape);
	shape = new Plane(Direction(0, 0, 1), Point(0, 0, 40), RGB(191, 191, 191)); //BACK
	list.push_back(shape);

	shape = new Parallelepiped(new Triangle(Point(15, -10, 20), Point(15, -20, 30), Point(15, -10, 40), RGB(255, 0, 0)),
					new Triangle(Point(15, -10, 20), Point(15, 0, 30), Point(15, -10, 40), RGB(0, 0, 180)), 4, RGB(255, 0, 0));
	list.push_back(shape);

 	// ******************************************* EL MARAVILLOSO PENE *********************************************************
	/*
	shape = new Sphere(Point(5, -13.5, 38), 7, RGB(252, 123, 220)); //RIGHT BALL
	list.push_back(shape);
	shape = new Sphere(Point(-5, -13.5, 38), 7, RGB(252, 123, 220)); //LEFT BALL
	list.push_back(shape);
	shape = new Cylinder(Disk(Direction(0, 1, 0), Point(0, -13, 35), 6, RGB(255, 255, 255)), 
				Disk(Direction(0,1,0), Point(0, 15, 35), 6, RGB(255,255,255)), RGB(214, 104, 187));
	list.push_back(shape);
	shape = new Sphere(Point(0, 15, 35), 6, RGB(173, 83, 151)); //TOP
	list.push_back(shape); */

	Camera camera = Camera(Point(0, 0, 0), Direction(0, 0, 10), Direction(10, 0, 0), 1080, 1080);	camera.setL(camera.getL() * -1);
	ofstream o("prueba.ppm");
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
  	uniform_real_distribution<float> distributionX = uniform_real_distribution<float>(0, planeVectorL.modulus());
  	uniform_real_distribution<float> distributionY = uniform_real_distribution<float>(0, planeVectorU.modulus());
	//camera.show();
	for(int i = 0; i < camera.getY(); i++){
		for(int j = 0; j < camera.getX(); j++){
			rndX = distributionX(gen); rndY = distributionY(gen);
			Point pointRnd = Point(aux[0] + rndX, aux[1] + rndY, aux[2]);
			d = (pointRnd +  (planeVectorL * j) + (planeVectorU * i))  - camera.getOrigin();
			d.normalize();
			for(int k = 0; k < list.size(); k++){	
				currentDist = list[k]->collision(d, camera.getOrigin(), collision);
				if(currentDist < minDist && collision && currentDist >= 0){
					index = k;
					minDist = currentDist;
				}
			}
			if(index != -1){
				o << (long int) list[index]->getColor().getR() << " " << (long int) list[index]->getColor().getG() << " " 
				  << (long int) list[index]->getColor().getB() << "	"; 
			}
			else{
				o << "0 0 0	";
			}
			index = -1;
			minDist = numeric_limits<float>::max();
		}
		o << endl;
	}
	o.close();
	return 0;
}