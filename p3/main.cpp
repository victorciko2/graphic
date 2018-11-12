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
	Shape *shape = new Sphere(Point(0, 0, 5), Direction(1, 0, 0), RGB(255, 0, 0));
	vector<Shape*> list;
	list.push_back(shape);
	shape = new Sphere(Point(0, 0, 7), Direction(2, 0, 0), RGB(0, 255, 0));
	list.push_back(shape);
	shape = new Sphere(Point(-20, -20, 50), Direction(9, 0, 0), RGB(255, 0, 255));
	list.push_back(shape);
	shape = new Plane(Direction(0, 0, 1), Point(0, 0, 240), RGB(120, 120, 120));
	Camera camera = Camera();
	ofstream o("prueba.ppm");
	o << "P3" << endl;
	o << "#" << "prueba.ppm" << endl;
	o << camera.getX() << " " << camera.getY() << endl;
	o << "255" << endl;
	Direction d;
	float minDist = numeric_limits<float>::max();
	int index = -1;
	for(int i = -camera.getX()/2; i < camera.getX()/2; i++){
		for(int j = -camera.getY()/2; j < camera.getY()/2; j++){
			d = Point(i, j, 100) - camera.getOrigin();
			for(int k = 0; k < list.size(); k++){
				if(list[k]->collision(d, camera.getOrigin()) < minDist 
					&& list[k]->collision(d, camera.getOrigin()) != -1){
					index = k;
					minDist = list[k]->collision(d, camera.getOrigin());
				}
			}
			if(index != -1){
				o << (long int) list[index]->getColor().getR() << " " << (long int) list[index]->getColor().getG() << " " 
				  << (long int) list[index]->getColor().getB() << "	";
			}
			else{
				o << "255 255 255	";
			}
			index = -1;
			minDist = numeric_limits<float>::max();
		}
	}
	o.close();
#endif
	return 0;
}