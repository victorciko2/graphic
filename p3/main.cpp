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
	Shape *shape = new Sphere(Point(-6, 0, 10), Direction(1, 0, 0), RGB(255, 0, 0));
	vector<Shape*> list;
	list.push_back(shape);
	shape = new Sphere(Point(0, 0, 17), Direction(2, 0, 0), RGB(0, 255, 0));
	//	list.push_back(shape);
	shape = new Sphere(Point(-6, -6, 16), Direction(3, 0, 0), RGB(255, 0, 255));
	list.push_back(shape);
	shape = new Plane(Direction(0, 0, 1), Point(0, 0, 20), RGB(0, 0, 255));
	list.push_back(shape);
	//shape = new Plane(Direction(0, 1, 0), Point(0, 240, 0), RGB(120, 0, 0));
	list.push_back(shape);
	//shape = new Plane(Direction(0, 1, 0), Point(0, -240, 0), RGB(120, 0, 0));
	list.push_back(shape);
	//shape = new Plane(Direction(1, 0, 0), Point(240, 0, 0), RGB(0, 120, 0));
	list.push_back(shape);
	shape = new Plane(Direction(1, 0, 0), Point(-240, 0, 0), RGB(0, 120, 0));
	//list.push_back(shape);
	Camera camera = Camera(Point(0, 0, 0), Direction(0, 0, 10), Direction(-10, 0, 0), Direction(0, 10, 0), 720);
	/*camera.setF(Direction(0, 0, 10));
	camera.setL(Direction(-10, 0, 0));	
	camera.setU(Direction(0, 10, 0));*/

	ofstream o("prueba.ppm");
	o << "P3" << endl;
	o << camera.getX() << " " << camera.getY() << endl;
	o << "255" << endl;
	Direction d;
	float minDist = numeric_limits<float>::max();
	int index = -1;
	Direction planeVectorL = camera.getL() * -2 / camera.getX();
	Direction planeVectorU = camera.getU() * -2 / camera.getY();
	Point aux = camera.getOrigin() + camera.getL() + camera.getU() + camera.getF() + planeVectorL / 2 + planeVectorU / 2;
	aux.show();	
	for(int i = 0; i < camera.getY(); i++){
		for(int j = 0; j < camera.getX(); j++){
			d = (aux + (planeVectorL * j) + (planeVectorU * i))  - camera.getOrigin();
			d.normalize();
			for(int k = 0; k < list.size(); k++){	
				if(list[k]->collision(d, camera.getOrigin()) < minDist
					&& list[k]->collision(d, camera.getOrigin()) != -1){ // llamarla solo una vez
					index = k;
					minDist = list[k]->collision(d, camera.getOrigin());
				}
			}
			if(index != -1){
				//cout << "mindist = " << minDist << endl;
				o << (long int) list[index]->getColor().getR() << " " << (long int) list[index]->getColor().getG() << " " 
				  << (long int) list[index]->getColor().getB() << "	";
			}
			else{
				o << "255 255 255	";
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