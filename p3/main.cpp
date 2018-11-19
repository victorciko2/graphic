#include <iostream>
#include <vector>
#include <fstream>	
#include <limits>
#include "shape.h"
#include "camera.h"

using namespace std;

int main(){	
#if 0
	Shape *shape = new Sphere(Point(), Direction(), RGB());
	vector<Shape*> list;
	Shape *shape2 = new Shape();
	list.push_back(shape);
	list.push_back(shape2);
	for (int i = 0; i < list.size(); i++){
		list[i]->show();
	}
#else
	vector<Shape*> list;
	Shape *shape = new Sphere(Point(-12, -13, 33), 7, RGB(255, 0, 255));//LEFT BALL
	list.push_back(shape);
	shape = new Sphere(Point(12, -13, 33), 7, RGB(0, 0, 255)); //RIGHT BALL
	list.push_back(shape);
	shape = new Sphere(Point(0, -13, 33-7), 7, RGB(255, 255, 0));//FRONT BALL
	list.push_back(shape);
	Camera camera = Camera(Point(0, 0, 0), Direction(0, 0, 10), Direction(10, 0, 0), 720, 720);	camera.setL(camera.getL() * -1);
	//camera.show();
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
	Point a (0,3,10);
	Point b (3,0,10);
	Point c (-3,0,10);

	shape = new Triangle(a,b,c,Plane((a-b)^(a-c),a, RGB(255,255,255)), RGB(255,255,255));
	
	

	list.push_back(shape);

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
	for(int i = 0; i < camera.getY(); i++){
		for(int j = 0; j < camera.getX(); j++){
			d = (aux + (planeVectorL * j) + (planeVectorU * i))  - camera.getOrigin();
			d.normalize();
			for(int k = 0; k < list.size(); k++){	
				currentDist = list[k]->collision(d, camera.getOrigin(), collision);
				if(currentDist < minDist && collision && currentDist > 0){
					index = k;
					minDist = currentDist;
					//cout << "collision con " << k << " a distancia " << currentDist << endl;
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
#endif
	return 0;
}