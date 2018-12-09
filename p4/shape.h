#ifndef SHAPE_H_
#define SHAPE_H_
#include <iostream>
#include <cstring>
#include <vector>
#include <random>
#include "point.h"
#include "direction.h"
#include "coordinateSystem.h"
#include "color.h"

using Vector = std::array<float,4>;
using Matrix = std::array<std::array<float, 4>, 4>;

class Point;

class Direction;

class Scene; 

class Shape; 

class Material{
protected:
	RGB color;
public:
	Material();
	Material(RGB color);
	RGB getColor();
	virtual RGB getColor(Direction n, Point origin, Point hit, Scene scene, int depth);
	virtual float getIntensity();
	virtual void show();
};

class BRDF : public Material{
protected:
	float kd, ks, alpha;//diffuse, specular, specular cone
	mt19937 gen;
	uniform_real_distribution<float> distribution;
public:
	BRDF();
	BRDF(float kd, float ks, float alpha, RGB color);
	float getKd();
	float getKs();
	float getAlpha();
	RGB getColor(Direction n, Point origin, Point hit, Scene scene, int depth);
	void show();

};

class Reflective : public Material{
public:
	Reflective();
	RGB getColor(Direction n, Point origin, Point hit, Scene scene, int depth);
	void show();
};

class Shape{
protected:
	Material* material;
	RGB color;
public:
	Shape();

	Shape(Material* material);

	Shape(RGB color);

	void setColor(RGB color);

	RGB getColor();

	RGB getColor(Direction n, Point origin, Point hit, Scene scene, int depth);

	float getIntensity();

	Material* getMaterial();

	void setMaterial(Material* material);

	virtual Direction getNormal(Point x);

	virtual float collision(Direction d, Point o, bool& collision);

	virtual string showAsString();

	virtual void show();
};

class Light : public Material{
protected:
	float p;
public:
	Light();
	Light(float p);
	Light(float p, RGB color);
	RGB getColor(Direction n, Point origin, Point hit, Scene scene, int depth);
	float getIntensity();
};

class PointLight : public Shape{
protected:
	Point o;
public:
	PointLight();
	PointLight(Point o, Light l); // Add Light as parameter?
	Point getOrigin();
};

class Scene{
protected:
	vector<Shape*> objects;
	vector<PointLight*> lights;
public:
	Scene();
	Scene(vector<Shape*> objects);
	Scene(vector<PointLight*> lights);
	Scene(vector<Shape*> objects, vector<PointLight*> lights);
	void add(Shape* s);
	void add(PointLight* l);
	vector<Shape*> getObjects();
	vector<PointLight*> getLights();
};

class Ray{
protected:
	Direction dir;
	Point p;
public:
	Ray(Direction dir, Point p);
	Direction getDirection();
	Point getPoint();
	Shape* collision(Scene scene, Point& intersection, float& dist);
	RGB tracePath(Scene scene, int depth);
};

class Sphere : public Shape{
private:
	Point center;
	float radius;
public:
	Sphere();

	Sphere(Point center, float radius, RGB color);

	Sphere(Point center, float radius, Material* material);

	void setCenter(Point center);

	void setRadius(float radius);

	void setColor(RGB color);

	void setMaterial(Material* material);

	Material* getMaterial();

	RGB getColor();

	Point getCenter();

	float getRadius();

	Direction getNormal(Point x);

	float collision(Direction d, Point o, bool& collision);

	string showAsString();
	
	void show();

	Sphere operator=(Sphere s);
};

class Plane : public Shape{
protected:
	Direction normal;
	Point o;
public:
	Plane();

	Plane(Direction normal, Point o, RGB color);

	Plane(Direction normal, Point o, Material* material);

	void setO(Point o);

	void setNormal(Direction normal);

	void setColor(RGB color);

	void setMaterial(Material* material);

	Material* getMaterial();

	RGB getColor();

	Point getO();

	Direction getNormal();

	Direction getNormal(Point x);

	float collision(Direction d, Point o, bool& collision);

	string showAsString();
	
	void show();

	Plane operator=(Plane p);
};

class Disk : public Plane {
protected:
	float radius;
public:
	Disk();

	Disk(Direction normal, Point o, float r, RGB color);
	
	Disk(Direction normal, Point o, float radius, Material* material);

	Point getO();

	float getRadius();

	Direction getNormal();

	RGB getColor();

	Material* getMaterial();

	void setMaterial(Material* material);

	void setO(Point o);

	void setRadius(float radius);

	void setColor(RGB color);

	void setNormal(Direction normal);

	float collision(Direction d, Point o, bool& collision);
};

class InfiniteCylinder : public Shape{
protected:
	Direction v; // Cylinder direction
	Point p; // point origin of v
	float radius;
public:
	InfiniteCylinder();

	InfiniteCylinder(Direction v, Point p, float r, RGB color);

	InfiniteCylinder(Direction v, Point p, float r, Material* material);
	
	Direction getDirection();

	Point getPoint();

	float getRadius();

	Material* getMaterial();

	void setMaterial(Material* material);

	RGB getColor();

	void setDirection(Direction v);

	void setPoint(Point p);

	void setRadius(float radius);

	Direction getNormal(Point x);

	void setColor(RGB color);

	float collision(Direction d, Point o, bool& collision);
};

class Cylinder : public InfiniteCylinder{
private:
	Plane sup;
	Plane inf;
	Disk top, bot;
public:
	Cylinder();

	Cylinder(Plane inf, float h, float radius, Direction v, Point p, RGB color);

	Cylinder(Plane inf, Plane sup, float radius, Direction v, Point p, RGB color);	

	Cylinder(Disk bot, Disk top, RGB color);

	Cylinder(Disk bot, Disk top, Material* material);

	Direction getDirection();

	Point getPoint();

	float getRadius();

	Material* getMaterial();

	void setMaterial(Material* material);

	RGB getColor();

	Plane getSup();

	Plane getInf();

	void setSup(Plane sup);

	void setInf(Plane inf);

	void setDirection(Direction d);

	void setPoint(Point p);

	void setRadius(float radius);

	void setColor(RGB color);

	float collision(Direction d, Point o, bool& collision);
};

class Triangle : public Shape{
private:
	Point a;
	Point b;
	Point c;
	Plane p;
public:
	Triangle();

	Triangle(Point a, Point b, Point c, Plane p, RGB color);

	Triangle(Point a, Point b, Point c, RGB color);

	Triangle(Point a, Point b, Point c, Plane p, Material* material);

	Triangle(Point a, Point b, Point c, Material* material);

	void setA(Point a);

	void setB(Point b);

	void setC(Point c);

	void setP(Plane p);

	void setColor(RGB color);

	RGB getColor();
	
	Material* getMaterial();

	void setMaterial(Material* material);

	Direction getNormal(Point x);

	Point getA();

	Point getB();

	Point getC();

	Plane getP();

	float collision(Direction d, Point o, bool& collision);

	string showAsString();

	void show();

	Triangle operator=(Triangle t);
};
bool solveQuadratic(float a, float b, float c, float& t0, float& t1);

#endif