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

	// Contar tiempo
	auto timeStart = std::chrono::high_resolution_clock::now();

	int	numPaths = atoi(argv[1]);
	int pixelX = atoi(argv[2]);
	int pixelY = atoi(argv[3]);
	string f = argv[4]; 
	Scene scene;
	Shape shape;
	vector<shared_ptr<Shape>> shapes;
	vector<shared_ptr<PointLight>> lights;

	// Emission ma = Emission(1500, RGB(1,1,1));
	// CORNELL BOX
	Plane left(Direction(1, 0, 0), Point(-10, 0, 10), make_shared<BRDF>(BRDF(0.7, 0, 100, RGB((float)255/255, 0, 0)))); //LEFT
	scene.add(make_shared<Plane>(left));

	Plane right(Direction(-1, 0, 0), Point(10, 0, -10), make_shared<BRDF>(BRDF(0.7, 0, 100, RGB(0, (float)255/255, 0)))); //RIGHT
	scene.add(make_shared<Plane>(right));

	Plane down(Direction(0, 1, 0), Point(0,-10, 10), make_shared<BRDF>(BRDF(0.7, 0, 100, RGB((float)230/255, (float)230/255, (float)230/255)))); //DOWN
	scene.add(make_shared<Plane>(down));

	Plane up(Direction(0, -1, 0), Point(0, 10, 10), make_shared<BRDF>(BRDF(0.7, 0, 100, RGB((float)230/255, (float)230/255, (float)230/255)))); //UP
	scene.add(make_shared<Plane>(up));

	Plane back(Direction(0, 0, -1), Point(0, 0, 20), make_shared<BRDF>(BRDF(0.7, 0, 100, RGB((float)191/255, (float)191/255, (float)191/255)))); //BACK
	scene.add(make_shared<Plane>(back));

	// scene.addAlbedo(RGB(1, 1, 1));
	// scene.addPM(0, 0);//absorcion and scattering (que putos datos poner)
	
	/*BRDF m = BRDF(0, 0.7, 100, RGB((float)59/255, (float)29/255, 0));
	*/

	BRDF m = BRDF(0.4, 0.4, 100, RGB((float)234/255, (float)190/255, (float)63/255));
	Sphere algo(Point(0, 0, 10), 3, make_shared<BRDF>(m));
	scene.add(make_shared<Sphere>(algo));

	/*Sphere algo2(Point(-7.5, 0, 10), 2, make_shared<Reflective>(m));
	scene.add(make_shared<Sphere>(algo2));

	Sphere algo3(Point(2.5, 0, 10), 2, make_shared<Reflective>(m));
	scene.add(make_shared<Sphere>(algo3));

	Sphere algo4(Point(7.5, 0, 10), 2, make_shared<Reflective>(m));
	scene.add(make_shared<Sphere>(algo4));*/


	/*Reflective m = Reflective();
	Cylinder rightArm(Disk(Direction(-1, 1, 0), Point(2, -1.5, 13), 4), 7, make_shared<Reflective>(m));
	scene.add(make_shared<Cylinder>(rightArm));*/

	/*Cylinder cyl1(Disk(Direction(0, 1, 0), Point(5, -10, 12), 0.15), 11.55, make_shared<BRDF>(m));
	scene.add(make_shared<Cylinder>(cyl1));

	Cylinder cyl2(Disk(Direction(0, 1, 0), Point(5, -10, 16), 0.15), 11.55, make_shared<BRDF>(m));
	scene.add(make_shared<Cylinder>(cyl2));

	Cylinder cyl3(Disk(Direction(0, 1, 0), Point(-5, -10, 12), 0.15), 11.55, make_shared<BRDF>(m));
	scene.add(make_shared<Cylinder>(cyl3));

	Cylinder cyl4(Disk(Direction(0, 1, 0), Point(-5, -10, 16), 0.15), 11.55, make_shared<BRDF>(m));
	scene.add(make_shared<Cylinder>(cyl4));*/

	//Cylinder cyl1(Disk(Direction(0, -1, 0), Point(7.5, -10, 12), 1), 4, make_shared<BRDF>(m));
	//scene.add(make_shared<Cylinder>(cyl1));

	/*BRDF m = BRDF(0.7, 0.1, 1, RGB((float)252/255, (float)123/255, (float)220/255));
	shape = Sphere(Point(2.5, -7, 17), 3, make_shared<Material>(m)); //RIGHT BALL
	//scene.add(make_shared<Shape>(shape));
	shape = Sphere(Point(-2.5, -7, 17), 3, make_shared<Material>(m)); //LEFT BALL
	//scene.add(make_shared<Shape>(shape));
	m = BRDF(0.7, 0.1, 1, RGB((float)214/255, (float)104/255, (float)187/255));

	shape = Cylinder(Disk(Direction(0, 1, 0), Point(0,-6.5, 18.5), 1.5), 
				Disk(Direction(0,1,0), Point(0, 8, 18), 3), make_shared<Material>(m));

	*/
	//Disk disquito(Direction(0, -1, 0), Point(0, 9.8, 10), 3, make_shared<Emission>(ma));
	// scene.add(make_shared<Disk>(disquito));
	
	/*m = BRDF(0.8, 0.1, 1, RGB((float)173/255, (float)83/255, (float)151/255));
	shape = Sphere(Point(0, 6.5, 17), 3, make_shared<Material>(m)); //TOP
	//scene.add(make_shared<Shape>(shape)); */

	//***************************SNOWMAN*******************************************
	/*Material ref = Reflective();
	//body
	*/
	/*Sphere body(Point(0,-7,13), 3, make_shared<BRDF>(m));
	scene.add(make_shared<Sphere>(body));*/
	
	/*Refractive m = Refractive(4, true);	
	Sphere body2(Point(0,-2.8,13), 2.5, make_shared<Refractive>(m));
	scene.add(make_shared<Sphere>(body2));*/
	/*
	Sphere body3(Point(0,0.4,13), 1.8, make_shared<BRDF>(m));
	//scene.add(make_shared<Sphere>(body3));
	m = BRDF(0.6, 0.1, 1, RGB((float)0/255, (float)0/255, (float)0/255));	
	//arms
	Cylinder rightArm(Disk(Direction(1, 1, 0), Point(2, -1.5, 13), 0.3), 4, make_shared<BRDF>(m));
	//scene.add(make_shared<Cylinder>(rightArm));
	Cylinder leftArm(Disk(Direction(-1, 1, 0), Point(-2, -1.5, 13), 0.3), 4, make_shared<BRDF>(m));
	//scene.add(make_shared<Cylinder>(leftArm));
	//eyes
	Sphere leftEye(Point(-0.7,0.8,11.2), 0.2, make_shared<BRDF>(m));
	//scene.add(make_shared<Sphere>(leftEye));
	Sphere rightEye(Point(0.7,0.8,11.2), 0.2, make_shared<BRDF>(m));
	//scene.add(make_shared<Sphere>(rightEye));
	//buttons
	Sphere button1(Point(0,-2.2,10), 0.4, make_shared<BRDF>(m));
	//scene.add(make_shared<Sphere>(button1));
	Sphere button2(Point(0,-1.1,10), 0.4, make_shared<BRDF>(m));
	//scene.add(make_shared<Sphere>(button2));
	Sphere button3(Point(0,-3.4,10), 0.4, make_shared<BRDF>(m));
	//scene.add(make_shared<Sphere>(button3));
	//mouth
	Cylinder mouth(Disk(Direction(1, 0, 0), Point(-0.5, -0.2, 11), 0.1), 1.1, make_shared<BRDF>(m));
	//scene.add(make_shared<Cylinder>(mouth));
	Parallelepiped paralele(new Triangle(Point(-8, -9.9, 14), Point(-8, -9.9, 18), Point(-2, -9.9, 18)),
					new Triangle(Point(-8, -9.9, 14), Point(-2, -9.9 ,14), Point(-2, -9.9, 18)), 12, make_shared<BRDF>(m));
	//scene.add(make_shared<Parallelepiped>(paralele));

	*/
	
	PointLight pl = PointLight(Point(0, 8, 10), make_shared<Light>(Light(250, RGB(1,1,1))));
	scene.add(make_shared<PointLight>(pl));

	PointLight pl = PointLight(Point(3.5, 8, 10), make_shared<Light>(Light(150, RGB(1,1,1))));
	scene.add(make_shared<PointLight>(pl));
	PointLight pl2 = PointLight(Point(-3.5, 8, 10), make_shared<Light>(Light(150, RGB(1,1,1))));
	scene.add(make_shared<PointLight>(pl2));

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
  	RGB mean;
  	Direction auxPi;
  	Point pointRnd;
  	Ray r;
  	int depth = 0;
  	RGB result;
  	float contador = 0;
	for(int i = 0; i < camera.getY(); i++){
		for(int j = 0; j < camera.getX(); j++){
			mean = RGB(0, 0, 0);
			for(int k = 0; k < numPaths; k++){
				rndX = distributionX(gen); rndY = distributionY(gen);
				pointRnd = Point(aux[0] + distributionX(gen)*planeVectorL.modulus(), 
										aux[1] + distributionX(gen)*planeVectorU.modulus(), aux[2]);
				auxPi = Direction(distributionX(gen)*2*camera.getL().modulus()/camera.getX(), distributionX(gen)*2*camera.getU().modulus()/camera.getY(), 0);
				d = (pointRnd +  (planeVectorL * j) + (planeVectorU * i))  - camera.getOrigin();
				d.normalize();
				//d.show();
				r = Ray(d, camera.getOrigin());
				depth = 0;
				result = r.tracePath(scene, depth);
				mean = RGB(mean[0] + result[0], mean[1] + result[1], mean[2] + result[2]);
			}
			mean = RGB((mean[0] / numPaths) * colorRange, (mean[1] / numPaths) * colorRange, (mean[2] / numPaths) * colorRange);
			if(mean[0] > colorRange) mean = RGB(colorRange, mean[1], mean[2]); 
			if(mean[1] > colorRange) mean = RGB(mean[0], colorRange, mean[2]);
			if(mean[2] > colorRange) mean = RGB(mean[0], mean[1], colorRange);
			if(mean[0] < 0) mean = RGB(0, mean[1], mean[2]); 
			if(mean[1] < 0) mean = RGB(mean[0], 0, mean[2]);
			if(mean[2] < 0) mean = RGB(mean[0], mean[1], 0);
			o << (int) mean[0] << " " << (int) mean[1] << " " << (int) mean[2] << " ";
		}
		o << endl;
		contador = (float) i;
		fprintf(stderr,"\r%5.2f%%", 100.*contador/(camera.getY()-1));
	}
	cout << endl;
	o.close();
	auto timeEnd = std::chrono::high_resolution_clock::now(); 
    auto passedTime = std::chrono::duration<double, std::milli>(timeEnd - timeStart).count(); 
    fprintf(stderr, "\rTiempo: %.2f (seg)\n", passedTime / 1000);
	return 0;
}
