<<<<<<< Updated upstream
#include <iostream>
#include <cmath>
#include <algorithm>
#include <limits>
#include "shape.h"

using namespace std;

Material::Material(){
	this->color = RGB(0, 0, 0);
}
Material::~Material(void){}

Material::Material(RGB color){
	this->color = color;
}

RGB Material::getColor(){
	return this->color;
}

RGB Material::getColor(Direction n, Point origin, Point hit, Scene scene, int depth){
	return this->color;
}

bool Material::getRefractive(){
	return false;
}

float Material::getIntensity(){
	return 0;
}

void Material::show(){
	this->color.show();

}

BRDF::BRDF(){
	this->kd = 0.7;
	this->ks = 0;
	this->alpha = 1;
	this->distribution = uniform_real_distribution<float>(0,1);
	random_device rd;
    // Initialize Mersenne Twister pseudo-random number generator
   	this->gen = mt19937 (rd());
   	this->color = RGB();
}

BRDF::~BRDF(void){}

BRDF::BRDF(float kd, float ks, float alpha, RGB color){
	this->kd = kd;
	this->ks = ks;
	this->alpha = alpha;
	this->distribution= uniform_real_distribution<float>(0,1);
	random_device rd;
    // Initialize Mersenne Twister pseudo-random number generator
   	this->gen = mt19937 (rd());
   	this->color = color;
}

float BRDF::getKd(){
	return this->kd;
}

float BRDF::getKs(){
	return this->ks;
}

float BRDF::getAlpha(){
	return this->alpha;
}

void BRDF::show(){
	this->color.show();
	cout << "kd: " << kd << endl;
	cout << "ks: " << ks << endl;
	cout << "alpha: " << alpha << endl;
}

bool BRDF::getRefractive(){
	return false;
}

const int maxDepth = 20;

RGB BRDF::getColor(Direction n, Point origin, Point hit, Scene scene, int depth){
	if(depth >= maxDepth){
		return {0, 0, 0};
	}
	//n.normalize(); //diria que hay que normalizarlo pero no se
	float rnd = this->distribution(gen);
	RGB dirLight(0, 0, 0);
	Direction wo = hit - origin; wo.normalize();
	//lighting 24
	Direction refPerfect = (wo - (n * (wo * n) * 2)); refPerfect.normalize();
	float Or = acos((n * wo) / (wo.modulus() * n.modulus()));
	RGB rgb = Material::getColor();
	//cout << "color material: " << rgb.showAsString() << endl;
	vector<shared_ptr<PointLight>> lights = scene.getLights();
	for(int i = 0; i < lights.size(); i++){
		PointLight pl = *lights[i];
		float dist = (hit - pl.getOrigin()).modulus();
		Direction dirAux = pl.getOrigin() - hit;
		dirAux.normalize();
		Ray r = Ray(dirAux, hit + dirAux * 0.1f); 
		Point collision;
		float shadowDist = -1;
		shared_ptr<Shape> object = r.collision(scene, collision, shadowDist); 
		if(object == nullptr || shadowDist > dist || shadowDist < 0 ){
			//cout << "not found object" << endl;
			float p = pl.getMaterial()->getIntensity();
			float a;
			Direction origin2hit = pl.getOrigin() - hit; origin2hit.normalize();
			if(dist < 1){
				a = p * abs(n * origin2hit);
			}
			else{
				a = abs(p / (dist * dist)) * abs(n * origin2hit);
			}
			RGB rgbL = pl.getMaterial()->getColor();
			dirLight = RGB(
				dirLight[0] + rgbL[0] * a * ((rgb[0]/M_PI) + ks * (alpha + 2) / 2 / M_PI * pow(abs(refPerfect * origin2hit), alpha)),
				dirLight[1] + rgbL[1] * a * ((rgb[1]/M_PI) + ks * (alpha + 2) / 2 / M_PI * pow(abs(refPerfect * origin2hit), alpha)),
				dirLight[2] + rgbL[2] * a * ((rgb[2]/M_PI) + ks * (alpha + 2) / 2 / M_PI * pow(abs(refPerfect * origin2hit), alpha)));
		}
		if(object != nullptr && object->getMaterial()->getRefractive()){
			dirAux = pl.getOrigin() - hit; dirAux.normalize();
			bool mierda;
			float dist2 = object->collision(dirAux, hit, mierda);
			float a;
			float p = pl.getMaterial()->getIntensity();
			Direction origin2hit = pl.getOrigin() - hit; origin2hit.normalize();
			if(dist < 1){
				a = p * abs(n * origin2hit);
			}
			else{
				a = abs(p / (dist * dist)) * abs(n * origin2hit);
			}
			RGB rgbL = object->getMaterial()->getColor(dirAux, hit, collision, scene, depth);
			dirLight = RGB(
				dirLight[0] + rgbL[0] * a * ((rgb[0]/M_PI) + ks * (alpha + 2) / 2 / M_PI * pow(abs(refPerfect * origin2hit), alpha)),
				dirLight[1] + rgbL[1] * a * ((rgb[1]/M_PI) + ks * (alpha + 2) / 2 / M_PI * pow(abs(refPerfect * origin2hit), alpha)),
				dirLight[2] + rgbL[2] * a * ((rgb[2]/M_PI) + ks * (alpha + 2) / 2 / M_PI * pow(abs(refPerfect * origin2hit), alpha)));
		}
	}
	if(maxDepth == 1){
		return dirLight;
	}
	//cout << "el color directo es : " << dirLight.showAsString() << endl; 
	if(rnd < kd){ //difuse
		//cout << "Difuse" << endl;
		Direction any(1, 1, 1); any.normalize();
		Direction axis = n ^ any; axis.normalize();
		CoordinateSystem coor(axis, n, axis^n, hit);
		float eO = distribution(gen);
		float eI = distribution(gen);
		float Oi = acos(sqrt(1 - eO));
		float Ai = 2 * M_PI * eI; //azimuth aleatorio
		Vector vecAux = {sin(Oi) * cos(Ai), cos(Oi), sin(Oi) * sin(Ai), 0};
		vecAux = coor.getM() * vecAux;
		Direction wi = Direction(vecAux[0], vecAux[1], vecAux[2]);
		wi.normalize();

		Ray dif = Ray(wi, hit + wi * 0.1f);
		RGB Li = dif.tracePath(scene, depth + 1);

		rgb = RGB((Li[0] * rgb[0]) + dirLight[0],
				   Li[1] * rgb[1] + dirLight[1],
				   Li[2] * rgb[2] + dirLight[2]);
	}
	else if(rnd < kd + ks){//specular
		//cout << "Specular" << endl; 
		Direction axis = n ^ refPerfect; axis.normalize();
		CoordinateSystem coor(axis, refPerfect, axis ^ refPerfect, hit);
		float eO = distribution(gen);
		float eI = distribution(gen);
		float Oi = acos(pow(eO, 1 / (alpha + 1)));
		float Ai = 2 * M_PI * eI;
		Vector vecAux = {sin(Oi)*cos(Ai), cos(Oi), sin(Oi) * sin(Ai), 0};
		vecAux = coor.getM() * vecAux;
		Direction wi = Direction(vecAux[0], vecAux[1], vecAux[2]);
		wi.normalize();

		Ray dif = Ray(wi, hit + wi * 0.1f);

		RGB Li = dif.tracePath(scene, depth + 1);

		float factor = abs(cos(Oi)) * (alpha+2) / (alpha + 1);
		rgb = RGB((Li[0] * rgb[0] * factor) + dirLight[0],
				   Li[1] * rgb[1] * factor + dirLight[1],
				   Li[2] * rgb[2] * factor + dirLight[2]); 
	}
	else{
		return RGB(0, 0, 0);
	}
	return rgb;
}

Light::Light(){
	this->color = RGB(1, 1, 1);
	this->p = 100;
}

Light::~Light(void){}

Light::Light(float p){
	this->p = p;
}

Light::Light(float p, RGB color){
	this->p = p;
	this->color = color;
}

Reflective::Reflective(){}

Reflective::~Reflective(void){}

RGB Reflective::getColor(Direction n, Point origin, Point hit, Scene scene, int depth){
	Direction wo = hit - origin; wo.normalize();
	float Or = acos((n * wo) / (wo.modulus() * n.modulus()));
	Direction wi = wo - (n * (wo * n) * 2); wi.normalize();
	Ray dif = Ray(wi, hit + wi * 0.1f);
	RGB Li = dif.tracePath(scene, depth + 1);
	return Li;
}

void Reflective::show(){
	this->color.show();
}

bool Reflective::getRefractive(){
	return false;
}

// I = incident ray, N = normal, iorm = index of refraction of medium, iorr = of the medium where the ray is
//CAMBIAR INDICES DE LOS MEDIOS
Direction refract(Direction I, Direction N, float iorm, float iorr, bool& refraction){
	I.normalize(); N.normalize();
    float cosi = I * N; 
    if(cosi < -1) cosi = -1;
    if(cosi > 1) cosi = 1;
    float etai = iorr, etat = iorm; 
    Direction n = N; 
    if (cosi < 0) { cosi = -cosi; } else { std::swap(etai, etat); n = N * -1; } 
    float eta = etai / etat; 
    float k = 1 - eta * eta * (1 - cosi * cosi); 
    Direction aux1 = I * eta; 
    Direction aux2 = (n * (eta * cosi - sqrtf(k)));
    aux1 = Direction(aux1[0] + aux2[0], aux1[1] + aux2[1], aux1[2] + aux2[2]);
    if(k < 0){
    	refraction = false;
    	return aux1;
    }
    else{
    	refraction = true;
    	return aux1;
    } 
} 

Refractive::Refractive(){this->n = 1; this->wide = false;}

Refractive::~Refractive(void){}

Refractive::Refractive(float n, bool wide){this->n = n; this->wide = wide;}

bool Refractive::getRefractive(){
	return true;
}

RGB Refractive::getColor(Direction n, Point origin, Point hit, Scene scene, int depth){
	if(!this->wide){
		Direction incident = hit - origin; incident.normalize();
		bool refraction;
		Direction outRay = refract(incident, n, this->n, 1, refraction); outRay.normalize();
		Ray r(outRay, hit + outRay * 0.1f);
		RGB Li = RGB(0, 0, 0);
		Point intersection; float dist;
		shared_ptr<Shape> shape = nullptr;
 		Li = r.tracePath(scene, depth + 1);
 		return Li;
	}
	else{
		//cout << "first refraction point " << hit.showAsString() << endl;
		Direction incident = hit - origin; incident.normalize();
		bool refraction;
		Direction outRay = refract(incident, n, this->n, 1, refraction); outRay.normalize();
		Ray r(outRay, hit + outRay * 0.1f);
		RGB Li = RGB(0, 0, 0);
		Point intersection; float dist;
		shared_ptr<Shape> shape = nullptr;
		shape = r.collision(scene, intersection,dist);
		//cout << "second : " << intersection.showAsString() << endl;
		if(shape != nullptr){
			//cout << "collision with" << endl;
			//shape->show(); cout << endl;
			Direction aux = (intersection - hit); aux.normalize();
			outRay = refract(aux, shape->getNormal(intersection), 1, this->n, refraction); outRay.normalize();
			Ray r(outRay, intersection + outRay * 0.1f);
			Li = r.tracePath(scene, depth + 1);
		}
		else{
			Li = r.tracePath(scene, depth + 1);
		}
		/*if(refraction)*/ Li = r.tracePath(scene, depth + 1);	
		return Li;
	}
}


//n is the surface normal
//color arriving to the surface from the light
RGB Light::getColor(Direction n, Point origin, Point hit, Scene scene, int depth){
	float dist = (origin - hit).modulus();
	Direction aux = origin - hit;
	aux.normalize();
	float a;
	n.normalize();
	if(dist < 1){ //lightning 17
		a = p * abs(n * aux);
	}
	else{
		a = abs(p / (dist * dist)) * 
				abs(n * aux);
	}
	RGB rgb = this->color;
	rgb = RGB(rgb[0] * a, rgb[1] * a, rgb[2] * a);
	return rgb;
}

float Light::getIntensity(){
	return this->p;
}

PointLight::PointLight(){}

PointLight::~PointLight(void){}

PointLight::PointLight(Point o, shared_ptr<Light> l) : Shape(l){
	this->o = o;
}

Point PointLight::getOrigin(){
	return this->o;
}

Scene::Scene(){}

Scene::~Scene(void){}

Scene::Scene(vector<shared_ptr<Shape>> objects){
	this->objects = vector<shared_ptr<Shape>>();
}

Scene::Scene(vector<shared_ptr<PointLight>> lights){
	this->lights = lights;
}

Scene::Scene(vector<shared_ptr<Shape>> objects, vector<shared_ptr<PointLight>> lights){
	this->objects = objects;
	this->lights = lights;
}

void Scene::add(shared_ptr<Shape> s){
	this->objects.push_back(s);
}

void Scene::add(shared_ptr<PointLight> l){
	this->lights.push_back(l);
}

vector<shared_ptr<Shape>> Scene::getObjects(){
	return this->objects;
}

vector<shared_ptr<PointLight>> Scene::getLights(){
	return this->lights;
}

Shape::Shape(){
	this->color = RGB();
	this->material = make_shared<Material>(Material());
}

Shape::~Shape(void){}
Shape::Shape(RGB color){
	this->color = color;
	this->material = make_shared<Material>(Material());
}

Shape::Shape(shared_ptr<Material> material){
	this->material = material;
	this->color = RGB();
}

void Shape::setColor(RGB color){
	this->color = color;
}

void Shape::setMaterial(shared_ptr<Material> material){
	this->material = material;
}

shared_ptr<Material> Shape::getMaterial(){
	return this->material;
}

RGB Shape::getColor(){
	return this->material->getColor();
}

RGB Shape::getColor(Direction n, Point origin, Point hit, Scene scene, int depth){
	return this->material->getColor(n, origin, hit, scene, depth);
}

Direction Shape::getNormal(Point x){
	return Direction(1, 0, 0);
}

float Shape::getIntensity(){
	return this->material->getIntensity();
}

float Shape::collision(Direction d, Point o, bool& collision){
	cout << "Failed to calculate collision" << endl;
	return -1;
}

string Shape::showAsString(){
	return "SHAPE:\n";
}

void Shape::show(){
	cout << this->showAsString() << endl;
}

Ray::Ray(){}
Ray::Ray(Direction dir, Point p){
	this->dir = dir;
	this->dir.normalize();
	this->p = p;
}

Ray::~Ray(void){}

Direction Ray::getDirection(){
	return this->dir;
}

Point Ray::getPoint(){
	return this->p;
}

shared_ptr<Shape> Ray::collision(Scene scene, Point& intersection, float& dist){
	float minDist = numeric_limits<float>::max();
	vector<shared_ptr<Shape>> es = scene.getObjects();
	Point minInter = Point();
	bool collision;
	shared_ptr<Shape> min = nullptr;
	float dist2 = 0;
	for(int i = 0; i < es.size(); i++){
		dist = 0;
		shared_ptr<Shape> o;
		o = es[i];
		dist2 = es[i]->collision(this->dir, this->p, collision);
		//es[i]->show();
		//cout << "a distancia: " << dist2 << " collision: " << collision << endl;
		if(collision && dist2 < minDist && dist2 > 0){
			//cout << "entro" << endl;
			minDist = dist2;
			minInter = this->dir * minDist + this->p;
			min = o;
		}
	}
	intersection = minInter;
	dist = minDist;
	return min;
}

RGB Ray::tracePath(Scene scene, int depth){
	float minDist;
	Point minInter;
	shared_ptr<Shape> min = collision(scene, minInter, minDist);
	if(min != nullptr){
		return min->getColor(min->getNormal(minInter),
				p, minInter, scene, depth);
	}
	else{
		return {0,0,0};
	}
}

Sphere::Sphere(){
	this->center = Point();
	this->radius = -1; //poner un float
	this->color = RGB();
	this->material = make_shared<Material>(Material());
}

Sphere::~Sphere(void){}

Sphere::Sphere(Point center, float radius, RGB color){
	this->center = center;
	this->color = color;
	this->radius = radius;
}

Sphere::Sphere(Point center, float radius, shared_ptr<Material> material){
	this->center = center;
	this->material = material;
	this->radius = radius;
}

Sphere::Sphere(Point center, float radius){
	this->center = center;
	this->radius = radius;
}

void Sphere::setCenter(Point center){
	this->center = center; 
}

void Sphere::setRadius(float radius){
	this->radius = radius;
}

void Sphere::setColor(RGB color){
	this->color = color;
}

void Sphere::setMaterial(shared_ptr<Material> material){
	this->material = material;
}

shared_ptr<Material> Sphere::getMaterial(){
	return this->material;
}

RGB Sphere::getColor(){
	return this->color;
}

Point Sphere::getCenter(){
	return this->center;
}

float Sphere::getRadius(){
	return this->radius;
}

Direction Sphere::getNormal(Point x){
	Direction result = (x - center);
	result.normalize();
	return result;
}

float Sphere::collision(Direction d, Point o, bool& collision){  // cambiar radio por float
	d.normalize();
	Direction aux = o - this->center;
	float a = d * d;
	float b = 2 * (d * aux);
	float c = aux * aux - this->radius * this->radius;
	float t0, t1;
	collision = solveQuadratic(a, b, c, t0, t1);
	if(t0 > t1) swap(t0, t1);
	if(collision){
		if(t0 < 0){
			t0 = t1;
			if(t0 < 0){
				return -1;
			}
		}
	}
	return t0;
}

string Sphere::showAsString(){
	string s = "SPHERE:\n Center: " + this->center.showAsString()
			+ "\nradius: " + to_string(this->radius)
			+ "\n Color:\n" + this->color.showAsString() + "\n";
	return s;
}

void Sphere::show(){
	cout << this->showAsString() << endl;
	this->material->show();
}
	
Sphere Sphere::operator=(Sphere s){
	this->center = s.getCenter();
	this->radius = s.getRadius();
	this->color = s.getColor();	
	return *this;
}

Plane::Plane(){
	this->o = Point();
	this->normal = Direction();
	this->color = RGB();
	this->material = make_shared<Material>(Material()); 
}

Plane::~Plane(void){}

Plane::Plane(Direction normal, Point o, RGB color){
	this->o = o;
	this->color = color;
	this->normal = normal;
}

Plane::Plane(Direction normal, Point o, shared_ptr<Material> material){
	this->o = o;
	this->material = material;
	this->normal = normal;
}

Plane::Plane(Direction normal, Point o){
	this->o = o;
	this->normal = normal;
}

void Plane::setO(Point o){
	this->o = o; 
}

void Plane::setNormal(Direction normal){
	this->normal = normal;
}

void Plane::setColor(RGB color){
	this->color = color;
}

void Plane::setMaterial(shared_ptr<Material> material){
	this->material = material;
}

shared_ptr<Material> Plane::getMaterial(){
	return this->material;
}

RGB Plane::getColor(){
	return this->color;
}

Point Plane::getO(){
	return this->o;
}

Direction Plane::getNormal(){
	return this->normal;
}

Direction Plane::getNormal(Point x){
	return this->normal;
}

float Plane::collision(Direction d, Point o, bool& collision){
	float t;
	d.normalize();
	Direction n = this->normal;
	n.normalize();
	float aux = n * d;
	if(abs(aux) > 0.00000001f){
		Direction l = this->o - o;
		//l.normalize();
		t = (l * n) / aux;
		if(t < 0){ //hemos tocado esto osea que ojito que igual esta mal
			collision = false;
			return -1;
		}
		else{
			collision = true;
			return t;
		}
	}
}

string Plane::showAsString(){
	string s = "PLANE:\n Point: " + this->o.showAsString()
			+ "\nnormal: " + this->normal.showAsString() + "\n";
	return s;
}

void Plane::show(){
	cout << this->showAsString() << endl;
	this->material->show(); cout << endl;
}
	
Plane Plane::operator=(Plane p){
	this->o = p.getO();
	this->normal = p.getNormal();
	this->color = p.getColor();	
	return *this;
}

Disk::Disk(){
	this->normal = Direction();
	this->o = Point();
	this->radius = -1;
	this->color = RGB();
	this->material = make_shared<Material>(Material());

}

Disk::~Disk(void){}

Disk::Disk(Direction normal, Point o, float radius, RGB color){
	this->normal = normal;
	this->radius = radius;
	this->o = o;
	this->color = color;
}

Disk::Disk(Direction normal, Point o, float radius, shared_ptr<Material> material){
	this->normal = normal;
	this->radius = radius;
	this->o = o;
	this->material = material;
}

Disk::Disk(Direction normal, Point o, float radius){
	this->normal = normal;
	this->radius = radius;
	this->o = o;
}

Point Disk::getO(){
	return this->o;
}

float Disk::getRadius(){
	return this->radius;
}

Direction Disk::getNormal(){
	return this->normal;
}

RGB Disk::getColor(){
	return this->color;
}

shared_ptr<Material> Disk::getMaterial(){
	return this->material;
}

void Disk::setMaterial(shared_ptr<Material> material){
	this->material = material;
}

void Disk::setO(Point o){
	this->o = o;
}

void Disk::setRadius(float radius){
	this->radius = radius;
}

void Disk::setColor(RGB color){
	this->color = color;
}

void Disk::setNormal(Direction normal){
	this->normal = normal;
}

float Disk::collision(Direction d, Point o, bool& collision){
	d.normalize();
	Plane aux = Plane(this->normal, this->o, this->color);
	float dist = aux.collision(d, o, collision);
	if(collision && dist >= 0){
		Point intersection = o + d * dist;
		if(abs((intersection - this->o).modulus()) <= this->radius){
			collision = true;
			return dist;
		}
		else{
			collision = false;
			return -1;
		}
	}
	else{
		collision = false;
		return -1;
	}
}

InfiniteCylinder::InfiniteCylinder(){
	this->v = Direction();
	this->p = Point();
	this->radius = -1;
	this->color = RGB();
	this->material = make_shared<Material>(Material());
}

InfiniteCylinder::~InfiniteCylinder(void){}

InfiniteCylinder::InfiniteCylinder(Direction v, Point p, float radius, RGB color){
	this->v = v;
	this->v.normalize();
	this->p = p;
	this->radius = radius;
	this->color = color;
}

InfiniteCylinder::InfiniteCylinder(Direction v, Point p, float radius, shared_ptr<Material> material){
	this->v = v;
	this->v.normalize();
	this->p = p;
	this->radius = radius;
	this->material = material;
}

InfiniteCylinder::InfiniteCylinder(Direction v, Point p, float radius){
	this->v = v;
	this->v.normalize();
	this->p = p;
	this->radius = radius;
}

Direction InfiniteCylinder::getDirection(){
	return this->v;
}

Point InfiniteCylinder::getPoint(){
	return this->p;
}

float InfiniteCylinder::getRadius(){
	return this->radius;
}

RGB InfiniteCylinder::getColor(){
	return this->color;
}

shared_ptr<Material> InfiniteCylinder::getMaterial(){
	return this->material;
}

void InfiniteCylinder::setMaterial(shared_ptr<Material> material){
	this->material = material;
}

void InfiniteCylinder::setDirection(Direction v){
	this->v = v;
	this->v.normalize();
}

void InfiniteCylinder::setPoint(Point p){
	this->p = p;
}

void InfiniteCylinder::setRadius(float radius){
	this->radius = radius;
}

void InfiniteCylinder::setColor(RGB color){
	this->color = color;
}

Direction InfiniteCylinder::getNormal(Point x){
	Direction aux = x - p;
	aux.normalize();
	aux = ((v ^ aux) ^ v);
	aux.normalize();
	return aux;
}

float InfiniteCylinder::collision(Direction d, Point o, bool& collision){
	float t0, t1,t;
	d.normalize();
	collision = true;
	Direction iP = o - this->p;
    float a = (d - (this->v * (d * this->v))) * (d - (this->v * (d * this->v))); 
    float b = 2 * ((d - this->v * (d * this->v)) * (iP - (this->v * (iP * this->v))));
    float c = (iP - (this->v * (iP * this->v))) * (iP - (this->v * (iP * this->v))) 
    			- this->radius * this->radius; 
    if (!solveQuadratic(a, b, c, t0, t1)){
    	collision = false;
    	return -1;
    }
    if (t0 > t1) std::swap(t0, t1);
    if (t0 < 0) { 
        t0 = t1; 
        if (t0 < 0){
        	collision = false;
        	return -1;
        }  
    }
    t = t0; 
	return t;
}

Cylinder::Cylinder(){
	this->v = Direction();
	this->p = Point();
	this->radius = -1;
	this->color = RGB();
	this->sup = Plane();
	this->inf = Plane();
	this->material = make_shared<Material>(Material());
}

Cylinder::~Cylinder(void){}

Cylinder::Cylinder(Plane inf, float h, float radius, Direction v, Point p, RGB color){
	this->v = inf.getNormal();
	this->p = inf.getO();
	this->radius = radius;
	this->color = color;
	this->inf = inf;
	sup = Plane(inf.getNormal() * -1, inf.getO() + inf.getNormal() * h,
			inf.getColor());
}

Cylinder::Cylinder(Plane inf, Plane sup, float radius, Direction v, Point p, RGB color){
	this->v = inf.getNormal();
	this->v.normalize();
	this->p = inf.getO();
	this->radius = radius;
	this->color = color;
	this->inf = inf;
	this->sup = sup; 
}

Cylinder::Cylinder(Disk bot, Disk top, RGB color){
	this->bot = bot;
	this->top = top;
	this->v = bot.getNormal();
	this->v.normalize();
	this->p = bot.getO();
	this->color = color;
	this->radius = top.getRadius();
} 

//bot normal must point to where the cylinder grows, but then must be point outwards
//for light interaction
Cylinder::Cylinder(Disk bot, float length, shared_ptr<Material> material){
	this->bot = bot;
	this->top = Disk(bot.getNormal(), bot.getO() + bot.getNormal() * length, bot.getRadius());
	this->v = bot.getNormal();
	this->bot.setNormal(bot.getNormal() * -1);
	this->v.normalize();
	this->p = bot.getO();
	this->material = material;
	this->radius = top.getRadius();
} 

Cylinder::Cylinder(Disk bot, float length){
	this->bot = bot;
	this->top = Disk(bot.getNormal(), bot.getO() * length, bot.getRadius());
	this->v = bot.getNormal();
	this->bot.setNormal(bot.getNormal() * -1);
	this->v.normalize();
	this->p = bot.getO();
	this->radius = bot.getRadius();
}

Cylinder::Cylinder(Disk bot, Disk top, shared_ptr<Material> material){
	this->bot = bot;
	this->top = top;
	this->v = top.getNormal();
	this->v.normalize();
	this->p = bot.getO();
	this->material = material;
	this->radius = top.getRadius();
}

Cylinder::Cylinder(Disk bot, Disk top){
	this->bot = bot;
	this->top = top;
	this->v = top.getNormal();
	this->v.normalize();
	this->p = bot.getO();
	this->radius = top.getRadius();
}

Direction Cylinder::getDirection(){
	return this->v;
}

Point Cylinder::getPoint(){
	return this->p;
}

float Cylinder::getRadius(){
	return this->radius;
}

RGB Cylinder::getColor(){
	return this->color;
}

shared_ptr<Material> Cylinder::getMaterial(){
	return this->material;
}

void Cylinder::setMaterial(shared_ptr<Material> material){
	this->material = material;
}

Plane Cylinder::getSup(){
	return this->sup;
}

Plane Cylinder::getInf(){
	return this->inf;
}

void Cylinder::setSup(Plane sup){
	this->sup = sup;
}

void Cylinder::setInf(Plane inf){
	this->inf = inf;
}

void Cylinder::setDirection(Direction v){
	this->v = v;
	this->v.normalize();
}

void Cylinder::setPoint(Point p){
	this->p = p;
}

void Cylinder::setRadius(float radius){
	this->radius = radius;
}

void Cylinder::setColor(RGB color){
	this->color = color;
}

float Cylinder::collision(Direction d, Point o, bool& collision){
	float dist, distCyl, distTop, distBot;
	d.normalize();	
	bool colCyl, colTop, colBot;
	InfiniteCylinder aux = InfiniteCylinder(this->v, this->p, this->radius, this->color);
	distCyl = aux.collision(d, o, colCyl);
	distTop = this->top.collision(d, o, colTop);
	distBot = this->bot.collision(d, o, colBot);
	if(!colCyl && !colTop && !colBot){
		collision = false;
		return -1;
	}
	else if(!colCyl && !colTop && colBot){
		collision = false;
		return -1;
	}
	else if(!colCyl && colTop && !colBot){
		collision = false;
		return -1;
	}
	else if(!colCyl && colTop && colBot){
		if(distTop < distBot){
			collision = true;
			return distTop;
		}
		else if(distBot <= distTop){
			collision = true;
			return distBot;
		}
		else{
			collision = false;
			return -1;
		}
	}
	else if(colCyl && !colTop && !colBot){
		float dist2Bot = ((o + d * distCyl) - bot.getO()).modulus();
		float dist2Top = ((o + d * distCyl) - top.getO()).modulus();
		if(dist2Bot <= dist2Top){
			if(sqrt(dist2Top * dist2Top - this->radius * this->radius) <= (this->top.getO() - this->bot.getO()).modulus()){
				collision = true;
				return distCyl;
			}
			else{
				collision = false;
				return -1;
			}
		}
		else{
			if(sqrt(dist2Bot * dist2Bot - this->radius * this->radius) <= (this->top.getO() - this->bot.getO()).modulus()){
				collision = true;
				return distCyl;
			}
			else{
				collision = false;
				return -1;
			}
		}
	}
	else if(colCyl && !colTop && colBot){
		if(distBot <= distCyl){
			collision = true;
			return distBot;
		}
		else{
			float dist2Top = ((o + d * distCyl) - top.getO()).modulus();
			if(sqrt(dist2Top * dist2Top - this->radius * this->radius) <= (this->top.getO() - this->bot.getO()).modulus()){
				collision = true; 
				return distCyl;
			}
			else{
				collision = true;
				return distBot;
			}
		}
	}
	else if(colCyl && colTop && !colBot){
		float dist2Bot = ((o + d * distCyl) - bot.getO()).modulus();
		if(sqrt(dist2Bot * dist2Bot - this->radius * this->radius) <= (this->top.getO() - this->bot.getO()).modulus()){
			collision = true;
			if(distTop < distCyl){
				return distTop;
			}
			else{
				return distCyl;
			}
		}
		else{
			collision = true;
			return distTop;
		}
	}
	else if(colCyl && colTop && colBot){
		if(distTop < distBot){
			collision = true;
			return distTop;
		}
		else if(distBot <= distTop){
			collision = true;
			return distBot;
		}
		else{
			collision = false;
			return -1;
		}
	}
	else{
		collision = false;
		return -1;
	}
}

Triangle::Triangle() {
	this->a = Point();
	this->b = Point();
	this->c = Point();
	this->p = Plane();
	this->color = RGB();
	this->material = make_shared<Material>(Material());
}

Triangle::~Triangle(void){}

Triangle::Triangle(Point a, Point b, Point c, RGB color) {
	this->a = a;
	this->b = b;
	this->c = c;
	Direction ab = a - b;
	Direction ac = a - c;
	Direction normal = ab ^ ac;
	normal.normalize();
	this->p = Plane(normal, this->a, this->color);
	this->color = color;
}

Triangle::Triangle(Point a, Point b, Point c, Plane p, RGB color) {
	this->a = a;
	this->b = b;
	this->c = c;
	this->p = p;
	this->color = color;
}

Triangle::Triangle(Point a, Point b, Point c, Plane p, shared_ptr<Material> material) {
	this->a = a;
	this->b = b;
	this->c = c;
	this->p = p;
	this->material = material;
}


Triangle::Triangle(Point a, Point b, Point c, shared_ptr<Material> material) {
	this->a = a;
	this->b = b;
	this->c = c;
	Direction ab = a - b;
	Direction ac = a - c;
	Direction normal = ab ^ ac;
	normal.normalize();
	normal = normal;
	this->p = Plane(normal, this->a, this->material);
	this->material = material;
}

Triangle::Triangle(Point a, Point b, Point c) {
	this->a = a;
	this->b = b;
	this->c = c;
	Direction ab = a - b;
	Direction ac = a - c;
	Direction normal = ab ^ ac;
	normal.normalize();
	normal = normal;
	this->p = Plane(normal, this->a, this->material);
}

void Triangle::setA(Point a){
	this->a = a;
}

void Triangle::setB(Point b){
	this->b = b;
}

void Triangle::setC(Point c){
	this->c = c;
}

void Triangle::setP(Plane p){
	this->p = p;
}

void Triangle::setColor(RGB color){
	this->color = color;
}

RGB Triangle::getColor(){
	return this->color;
}

shared_ptr<Material> Triangle::getMaterial(){
	return this->material;
}

void Triangle::setMaterial(shared_ptr<Material> material){
	this->material = material;
}

Point Triangle::getA(){
	return this->a;
}

Point Triangle::getB(){
	return this->b;
}

Point Triangle::getC(){
	return this->c;
}

Plane Triangle::getP(){
	return this->p;
}

Direction Triangle::getNormal(Point x){
	return this->p.getNormal(x);
}

float Triangle::collision(Direction d, Point o, bool& collision){
	float dist=this->p.collision(d,o,collision);
	if(dist > 0){
		//cout << " colisiona con el plano" << endl;
		Direction edge0 = this->b - this->a; 
		Direction edge1 = this->c - this->b; 
		Direction edge2 = this->a - this->c; 
		Direction C0 = (o+d*dist) - this->a; 
		Direction C1 = (o+d*dist) - this->b; 
		Direction C2 = (o+d*dist) - this->c; 
		if (p.getNormal()*(edge0^C0) > 0 && p.getNormal()*(edge1^C1) > 0 && p.getNormal()*(edge2^C2) > 0){
			collision = true;
			return dist;
		}
		else{
			collision = false;
			return -1;
		}
	}
	else{
		collision = false;
		return -1;
	}
}
	
string Triangle::showAsString(){
	string s = "TRIANGLE:\n Point A: " + this->a.showAsString()
			+ "\nPoint B: " + this->b.showAsString()
			+ "\nPoint C: " + this->c.showAsString();
	return s;
}

void Triangle::show(){
	cout << this->showAsString() << endl;
}

Triangle Triangle::operator=(Triangle t){
	this->a = t.getA();
	this->b = t.getB();
	this->c = t.getC();
	this->color = t.getColor();	
	return *this;
}

Parallelepiped::Parallelepiped(Triangle* A, Triangle* B, float c, RGB color){
	// Triangle A: b point alone, a and c shared with triangle B
	// Triangle B: b point alone, a and c shared with triangle A
	this->color = color;
	this->t1 = A;
	this->t2 = B;

	Point Aa = A->getA();
	Point Ab = A->getB();
	Point Ac = A->getC();

	Point Ba = B->getA();
	Point Bb = B->getB();
	Point Bc = B->getC();

	Direction ab = Aa - Ab;
	Direction bc = Ab - Ac;
	Direction normal = ab ^ bc;
	normal.normalize();

	// Compute of the 4 points with the normal and the distance
	this->p = normal*c+Ab;
	this->q = normal*c+Ac;
	this->r = normal*c+Aa;
	this->s = normal*c+Bb;

	this->t3 = new Triangle(Aa, r, s, RGB(255, 0, 111));
	this->t4 = new Triangle(Aa, Bb, s, RGB(125, 255, 125));
	this->t5 = new Triangle(Ac, Bb, s, RGB(255, 0, 0));
	this->t6 = new Triangle(Ac, s, q, RGB(255, 0, 0));
	this->t7 = new Triangle(Ab, p, q, RGB(255, 0, 0));
	this->t8 = new Triangle(Ab, Ac, q, RGB(255, 0, 0));
	this->t9 = new Triangle(Ab, r, p, RGB(255, 0, 255));
	this->t10 = new Triangle(Ab, Aa, r, RGB(255, 255, 0));
	this->t11 = new Triangle(r, s, p, RGB(255, 125, 0));
	this->t12 = new Triangle(s, p, q, RGB(255, 0, 0));
}

Parallelepiped::~Parallelepiped(void){}

Parallelepiped::Parallelepiped(Triangle* A, Triangle* B, float c, shared_ptr<Material> material){
	// Triangle A: b point alone, a and c shared with triangle B
	// Triangle B: b point alone, a and c shared with triangle A
	this->color = color;
	this->t1 = A;
	this->t2 = B;
	this->material = material;

	Point Aa = A->getA();
	Point Ab = A->getB();
	Point Ac = A->getC();

	Point Ba = B->getA();
	Point Bb = B->getB();
	Point Bc = B->getC();

	Direction ab = Aa - Ab;
	Direction bc = Ab - Ac;
	Direction normal = ab ^ bc;
	normal.normalize();

	// Compute of the 4 points with the normal and the distance
	this->p = normal*c+Ab;
	this->q = normal*c+Ac;
	this->r = normal*c+Aa;
	this->s = normal*c+Bb;

	this->t3 = new Triangle(Aa, r, s, this->material);
	this->t4 = new Triangle(Aa, Bb, s, this->material);
	this->t5 = new Triangle(Ac, Bb, s, this->material);
	this->t6 = new Triangle(Ac, s, q, this->material);
	this->t7 = new Triangle(Ab, p, q, this->material);
	this->t8 = new Triangle(Ab, Ac, q, this->material);
	this->t9 = new Triangle(Ab, r, p, this->material);
	this->t10 = new Triangle(Ab, Aa, r, this->material);
	this->t11 = new Triangle(r, s, p, this->material);
	this->t12 = new Triangle(s, p, q, this->material);
}

Triangle* normalParallelepiped = new Triangle();
int triangle = -1;
float Parallelepiped::collision(Direction d, Point o, bool& collision){
	vector<float> distances;
	d.normalize();
	distances.push_back(t1->collision(d, o, collision));
	distances.push_back(t2->collision(d, o, collision));
	distances.push_back(t3->collision(d, o, collision));
	distances.push_back(t4->collision(d, o, collision));
	distances.push_back(t5->collision(d, o, collision));
	distances.push_back(t6->collision(d, o, collision));
	distances.push_back(t7->collision(d, o, collision));
	distances.push_back(t8->collision(d, o, collision));
	distances.push_back(t9->collision(d, o, collision));
	distances.push_back(t10->collision(d, o, collision));
	distances.push_back(t11->collision(d, o, collision));
	distances.push_back(t12->collision(d, o, collision));

	float minDist = numeric_limits<float>::max();
	float distance = 0.0;
	collision = false;
	for (int i = 0; i < distances.size(); i++){
		distance = distances[i];
		if (distance < minDist && distance != -1){
			minDist = distance;
			triangle = i;
			collision = true;
		}
	}
	if(collision){
		/*if(triangle == 0){
			normalParallelepiped = this->t1;
		}
		else if(triangle == 1){
			normalParallelepiped = this->t2;
		}
		else if(triangle == 2){
			normalParallelepiped = this->t3;
		}
		else if(triangle == 3){
			normalParallelepiped = this->t4;
		}
		else if(triangle == 4){
			normalParallelepiped = this->t5;
		}
		else if(triangle == 5){
			normalParallelepiped = this->t6;
		}
		else if(triangle == 6){
			normalParallelepiped = this->t7;
		}
		else if(triangle == 7){
			normalParallelepiped = this->t8;
		}
		else if(triangle == 8){
			normalParallelepiped = this->t9;
		}
		else if(triangle == 9){
			normalParallelepiped = this->t10;
		}
		else if(triangle == 10){
			normalParallelepiped = this->t11;
		}
		else if(triangle == 11){
			normalParallelepiped = this->t12;
		}*/
		
		return minDist;
	}
	else{
		return -1;
	}
}

void Parallelepiped::setColor(RGB color){
	this->color = color;
}

RGB Parallelepiped::getColor(){
	return this->color;
}

Direction Parallelepiped::getNormal(Point x){
	Direction d;
	if(triangle == 0 || triangle == 1){
		d = this->t1->getA() - this->r ;
	}
	else if(triangle == 2 || triangle == 3){
		d = this->r - this->p;
	}
	else if(triangle == 4 || triangle == 5){
		d = this->s - this->r;
	}
	else if(triangle == 6 || triangle == 7){
		d = this->q - this->s;
	}
	else if(triangle == 8 || triangle == 9){
		d = this->r - this->s;
	}
	else if(triangle == 10 || triangle == 11){
		d = this->r - this->t1->getA();
	}
	d.normalize();
	return d;
}	

string Parallelepiped::showAsString(){
	string s = "Parallelepiped: \n";
	/*s += this->t1->showAsString();
	s += this->t2->showAsString();
	s += this->t3->showAsString();
	s += this->t4->showAsString();
	s += this->t5->showAsString();
	s += this->t6->showAsString();
	s += this->t7->showAsString();
	s += this->t8->showAsString();
	s += this->t9->showAsString();
	s += this->t10->showAsString();
	s += this->t11->showAsString();
	s += this->t12->showAsString();*/
	return s;
}

void Parallelepiped::show(){
	cout << this->showAsString() << endl;
}

bool solveQuadratic(float a, float b, float c, float& t0, float& t1){
	float discr = b * b - 4 * a * c; //discriminant
	if(discr < 0){ //doesnt have solution
		return false;
	}
	else if(discr == 0){//one solution
		t0 = -b / (2 * a);
		t1 = t0;
	}
	else{//two solutions
		t0 = (-b + sqrt(discr)) / (2 * a);
		t1 = (-b - sqrt(discr)) / (2 * a); 
	}
	if(t0 > t1){
		swap(t0, t1);
	}
	return true;
=======
#include <iostream>
#include <cmath>
#include <limits>
#include "shape.h"

using namespace std;

Material::Material(){
	this->color = RGB(0, 0, 0);
}

Material::Material(RGB color){
	this->color = color;
}

RGB Material::getColor(){
	return this->color;
}

RGB Material::getColor(Direction n, Point origin, Point hit, Scene scene, int depth){
	return this->color;
}

float Material::getIntensity(){
	return 0;
}

void Material::show(){
	cout << "entroo aqui"<< endl;
	this->color.show();

}

BRDF::BRDF(){
	this->kd = 0.7;
	this->ks = 0;
	this->alpha = 1;
	this->distribution= uniform_real_distribution<float>(0,1);
	random_device rd;
    // Initialize Mersenne Twister pseudo-random number generator
   	this->gen = mt19937 (rd());
   	this->color = RGB();
}

BRDF::BRDF(float kd, float ks, float alpha, RGB color){
	this->kd = kd;
	this->ks = ks;
	this->alpha = alpha;
	this->distribution= uniform_real_distribution<float>(0,1);
	random_device rd;
    // Initialize Mersenne Twister pseudo-random number generator
   	this->gen = mt19937 (rd());
   	this->color = color;
}

float BRDF::getKd(){
	return this->kd;
}

float BRDF::getKs(){
	return this->ks;
}

float BRDF::getAlpha(){
	return this->alpha;
}

void BRDF::show(){
	this->color.show();
	cout << "kd: " << kd << endl;
	cout << "ks: " << ks << endl;
	cout << "alpha: " << alpha << endl;
}

int times = 0;
const int maxDepth = 20;

RGB BRDF::getColor(Direction n, Point origin, Point hit, Scene scene, int depth){
	cout << "------------++++++++"<<endl;
	if(depth >= maxDepth){
		return {0, 0, 0};
	}
	times++;
	float rnd = this->distribution(gen);
	RGB dirLight(0, 0, 0);
	Direction wo = hit - origin; wo.normalize();
	//lighting 24
	Direction refPerfect = (wo - (n * (wo * n) * 2)); refPerfect.normalize();
	float Or = acos((n * wo) / (wo.modulus() * n.modulus()));
	RGB rgb = this->color;
	vector<PointLight*> lights = scene.getLights();
	for(int i = 0; i < lights.size(); i++){
		PointLight pl = *lights[i];
		float dist = (hit - pl.getOrigin()).modulus();
		Direction dirAux = pl.getOrigin() - hit;
		dirAux.normalize();
		Ray r = Ray(dirAux, hit + dirAux * 0.1f);
		Point collision;
		float shadowDist = -1;
		Shape* object = r.collision(scene, collision, shadowDist); 
		//incoming light to material
		if(object != nullptr && shadowDist > 0 && shadowDist < dist){
			float p = pl.getMaterial().getIntensity();
			float a;
			Direction aux = pl.getOrigin() - hit;
			aux.normalize();
			if(dist < 1){
				a = p * abs(n * aux);
			}
			else{
				a = abs(p / (dist * dist)) * abs(n * aux);
			}
			RGB rgbL = pl.getMaterial().getColor();
			Direction origin2hit = pl.getOrigin() - hit; origin2hit.normalize();
			float dirLightR = dirLight[0] + rgbL[0] * a * ((rgb[0]/M_PI) + ks * (alpha + 2) / 2
						 / M_PI * pow(abs(refPerfect * origin2hit), alpha));
			float dirLightG = dirLight[1] + rgbL[1] * a * ((rgb[1]/M_PI) + ks * (alpha + 2) / 2
						 / M_PI * pow(abs(refPerfect * origin2hit), alpha));
			float dirLightB = dirLight[2] + rgbL[2] * a * ((rgb[2]/M_PI) + ks * (alpha + 2) / 2
						 / M_PI * pow(abs(refPerfect * origin2hit), alpha));
			dirLight = RGB(dirLightR, dirLightG, dirLightB);
		}
	}
	lights = scene.getDlights();
	for(int i = 0; i < lights.size(); i++){
		DiskLight dl = *lights[i];
		float dist = (hit - dl.getPoint()).modulus();
		Direction dirAux = dl.getPoint() - hit;
		dirAux.normalize();
		Ray r = Ray(dirAux, hit + dirAux * 0.1f);
		Point collision;
		float shadowDist = -1;
		Shape* object = r.collision(scene, collision, shadowDist); 
		//incoming light to material
		if(object != nullptr && shadowDist > 0 && shadowDist < dist){
			float p = dl.getMaterial().getIntensity();
			float a;
			Direction aux = dl.getPoint() - hit;
			aux.normalize();
			if(dist < 1){
				a = p * abs(n * aux);
			}
			else{
				a = abs(p / (dist * dist)) * abs(n * aux);
			}
			RGB rgbL = dl.getMaterial().getColor();
			Direction origin2hit = dl.getPoint() - hit; origin2hit.normalize();
			float dirLightR = dirLight[0] + rgbL[0] * a * ((rgb[0]/M_PI) + ks * (alpha + 2) / 2
						 / M_PI * pow(abs(refPerfect * origin2hit), alpha));
			float dirLightG = dirLight[1] + rgbL[1] * a * ((rgb[1]/M_PI) + ks * (alpha + 2) / 2
						 / M_PI * pow(abs(refPerfect * origin2hit), alpha));
			float dirLightB = dirLight[2] + rgbL[2] * a * ((rgb[2]/M_PI) + ks * (alpha + 2) / 2
						 / M_PI * pow(abs(refPerfect * origin2hit), alpha));
			dirLight = RGB(dirLightR, dirLightG, dirLightB);
		}
	}
	vector<Shape*> objects = scene.getObjects();
	bool rayShot = false;
	if(rnd < kd){ //difuse
		rayShot = true;
		Direction any(1, 1, 1);
		Direction axis = n ^ any; axis.normalize();
		CoordinateSystem coor(axis, n, axis^n, hit);
		float eO = distribution(gen);
		float eI = distribution(gen);
		float Oi = acos(sqrt(1 - eO));
		float Ai = 2 * M_PI * eI;
		Vector vecAux = {sin(Oi) * cos(Ai), cos(Oi), sin(Oi) * sin(Ai), 0};
		vecAux = coor.getM() * vecAux;
		Direction wi = Direction(vecAux[0], vecAux[1], vecAux[2]);
		wi.normalize();

		Ray dif = Ray(wi, hit + wi * 0.1f);
		RGB Li = dif.tracePath(scene, depth + 1);

		rgb = RGB((Li[0] * rgb[0]) + dirLight[0],
				   Li[1] * rgb[1] + dirLight[1],
				   Li[2] * rgb[2] + dirLight[2]);
	}
	else if(rnd < kd + ks){//specular
		rayShot = true;
		Direction axis = n ^ refPerfect; axis.normalize();
		CoordinateSystem coor(axis, refPerfect, axis ^ refPerfect, hit);
		float eO = distribution(gen);
		float eI = distribution(gen);
		float Oi = acos(pow(eO, 1 / (alpha + 1)));
		float Ai = 2 * M_PI * eI;
		Vector vecAux = {sin(Oi)*cos(Ai), cos(Oi), sin(Oi) * sin(Ai), 0};
		vecAux = coor.getM() * vecAux;
		Direction wi = Direction(vecAux[0], vecAux[1], vecAux[2]);
		wi.normalize();

		Ray dif = Ray(wi, hit + wi * 0.1f);

		RGB Li = dif.tracePath(scene, depth + 1);

		float factor = abs(cos(Oi)) * (alpha+2) / (alpha + 1);
		rgb = RGB((Li[0] * rgb[0] * factor) + dirLight[0],
				   Li[1] * rgb[1] * factor + dirLight[1],
				   Li[2] * rgb[2] * factor + dirLight[2]); 
	}
	if(rayShot){
		return rgb;
	}
	else{
		return RGB(0, 0, 0);
	}
}

Light::Light(){
	this->color = RGB(1, 1, 1);
	this->p = 100;
}

Light::Light(float p){
	this->p = p;
}

Light::Light(float p, RGB color){
	this->p = p;
	this->color = color;
}

Reflective::Reflective(){}

RGB Reflective::getColor(Direction n, Point origin, Point hit, Scene scene, int depth){
	Direction wo = hit - origin; wo.normalize();
	float Or = acos((n * wo) / (wo.modulus() * n.modulus()));
	Direction wi = wo - (n * (wo * n) * 2); wi.normalize();
	Ray dif = Ray(wi, hit + wi * 0.1f);
	RGB Li = dif.tracePath(scene, depth + 1);
	return Li;
}

void Reflective::show(){
	this->color.show();
}

//n is the surface normal
//color arriving to the surface from the light
RGB Light::getColor(Direction n, Point origin, Point hit, Scene scene, int depth){
	float dist = (origin - hit).modulus();
	Direction aux = origin - hit;
	aux.normalize();
	float a;
	n.normalize();
	if(dist < 1){ //lightning 17
		a = p * abs(n * aux);
	}
	else{
		a = abs(p / (dist * dist)) * 
				abs(n * aux);
	}
	RGB rgb = this->color;
	rgb = RGB(rgb[0] * a, rgb[1] * a, rgb[2] * a);
	return rgb;
}

float Light::getIntensity(){
	return this->p;
}

PointLight::PointLight(){}

PointLight::PointLight(Point o, Light l) : Shape(l){
	this->o = o;
}

Point PointLight::getOrigin(){
	return this->o;
}

DiskLight::DiskLight(Disk d, Light l) : Shape(l){
	this->d = d;
}

Point diskLight::getPoint(){
	return this->o;
}


Scene::Scene(){}

Scene::Scene(vector<Shape*> objects){
	this->objects = vector<Shape*>();
}

Scene::Scene(vector<PointLight*> lights){
	this->lights = lights;
}

Scene::Scene(vector<Shape*> objects, vector<PointLight*> lights){
	this->objects = objects;
	this->lights = lights;
}

void Scene::add(Shape* s){
	this->objects.push_back(s);
}

void Scene::add(PointLight* l){
	this->lights.push_back(l);
}

vector<Shape*> Scene::getObjects(){
	return this->objects;
}

vector<PointLight*> Scene::getLights(){
	return this->lights;
}

Shape::Shape(){
	this->color = RGB();
	this->material = new Material();
}

Shape::Shape(RGB color){
	this->color = color;
	this->material = new Material();
}

Shape::Shape(Material* material){
	this->material = material;
	this->color = RGB();
}

void Shape::setColor(RGB color){
	this->color = color;
}

void Shape::setMaterial(Material* material){
	this->material = material;
}

Material* Shape::getMaterial(){
	return this->material;
}

RGB Shape::getColor(){
	return this->material->getColor();
}

RGB Shape::getColor(Direction n, Point origin, Point hit, Scene scene, int depth){
	return this->material->getColor(n, origin, hit, scene, depth);
}

Direction Shape::getNormal(Point x){
	return Direction(1, 0, 0);
}

float Shape::getIntensity(){
	return this->material->getIntensity();
}

float Shape::collision(Direction d, Point o, bool& collision){
	cout << "Failed to calculate collision" << endl;
	return -1;
}

string Shape::showAsString(){
	return "SHAPE:\n";
}

void Shape::show(){
	cout << this->showAsString() << endl;
}

Ray::Ray(Direction dir, Point p){
	this->dir = dir;
	this->dir.normalize();
	this->p = p;
}

Direction Ray::getDirection(){
	return this->dir;
}

Point Ray::getPoint(){
	return this->p;
}

Shape* Ray::collision(Scene scene, Point& intersection, float& dist){
	float minDist = numeric_limits<float>::max();
	vector<Shape*> es = scene.getObjects();
	Point minInter = Point();
	bool collision;
	Shape* min = nullptr;
	float dist2 = 0;
	for(int i = 0; i < es.size(); i++){
		dist = 0;
		Shape* o;
		o = es[i];
		dist2 = es[i]->collision(this->dir, this->p, collision);
		//es[i]->show();
		//cout << "a distancia: " << dist2 << " collision: " << collision << endl;

		if(collision && dist2 < minDist && dist2 > 0){
		//	cout << "entro" << endl;
			minDist = dist2;
			minInter = this->dir * minDist + this->p;
			min = o;
		}
	}
	intersection = minInter;
	dist = minDist;
	return min;
}

RGB Ray::tracePath(Scene scene, int depth){
	float minDist;
	Point minInter;
	Shape* min = collision(scene, minInter, minDist);
	if(min != nullptr){
	//cout << "collisiona " << min << endl;
		return min->getColor(min->getNormal(minInter),
				p, minInter, scene, depth);
	}
	else{
		return {0,0,0};
	}
}

Sphere::Sphere(){
	this->center = Point();
	this->radius = -1; //poner un float
	this->color = RGB();
	this->material = new Material();
}

//A LO MEJOR EL COORDINATE SYSTEM ESTA MAL CALCULADO POR LA CIUDAD DE REFERENCIA
Sphere::Sphere(Point center, float radius, RGB color){
	this->center = center;
	this->color = color;
	this->radius = radius;
}

Sphere::Sphere(Point center, float radius, Material* material) : Shape(material){
	this->center = center;
	this->material = material;
	this->radius = radius;
}

void Sphere::setCenter(Point center){
	this->center = center; 
}

void Sphere::setRadius(float radius){
	this->radius = radius;
}

void Sphere::setColor(RGB color){
	this->color = color;
}

void Sphere::setMaterial(Material* material){
	this->material = material;
}

Material* Sphere::getMaterial(){
	return this->material;
}

RGB Sphere::getColor(){
	return this->color;
}

Point Sphere::getCenter(){
	return this->center;
}

float Sphere::getRadius(){
	return this->radius;
}

Direction Sphere::getNormal(Point x){
	Direction result = x - center;
	result.normalize();
	return result;
}

float Sphere::collision(Direction d, Point o, bool& collision){  // cambiar radio por float
	d.normalize();
	float a = d * d;
	float b = 2 * (d * (o - this->center));
	float c = (o - this->center)*(o - this->center) - radius * radius;
	float t0, t1;
	collision = solveQuadratic(a, b, c, t0, t1);
	if(collision){
		if(t0 > 0){
			return t0;
		}
		else{
			return -1;
		}
	}
	else{
		return -1;
	}
}

string Sphere::showAsString(){
	string s = "SPHERE:\n Center: " + this->center.showAsString()
			+ "\nradius: " + to_string(this->radius)
			+ "\n Color:\n" + this->color.showAsString();
	return s;
}

void Sphere::show(){
	cout << this->showAsString() << endl;
	this->material->show();
}
	
Sphere Sphere::operator=(Sphere s){
	this->center = s.getCenter();
	this->radius = s.getRadius();
	this->color = s.getColor();	
	return *this;
}

Plane::Plane(){
	this->o = Point();
	this->normal = Direction();
	this->color = RGB();
	this->material = new Material(); 
}

Plane::Plane(Direction normal, Point o, RGB color){
	this->o = o;
	this->color = color;
	this->normal = normal;
}

Plane::Plane(Direction normal, Point o, Material* material){
	this->o = o;
	this->material = material;
	this->normal = normal;
}

void Plane::setO(Point o){
	this->o = o; 
}

void Plane::setNormal(Direction normal){
	this->normal = normal;
}

void Plane::setColor(RGB color){
	this->color = color;
}

void Plane::setMaterial(Material* material){
	this->material = material;
}

Material* Plane::getMaterial(){
	return this->material;
}

RGB Plane::getColor(){
	return this->color;
}

Point Plane::getO(){
	return this->o;
}

Direction Plane::getNormal(){
	return this->normal;
}

Direction Plane::getNormal(Point x){
	return this->normal;
}

float Plane::collision(Direction d, Point o, bool& collision){
	float t;
	d.normalize();
	Direction n = this->normal;
	n.normalize();
	float aux = d * n;
	if(abs(aux) > 0.00000001f){
		Direction l = this->o - o;
		t = (l * n) / aux;
		if(t < 0){//hemos tocado esto osea que ojito que igual esta mal
			collision = false;
			return -1;
		}
		collision = true;
		return t;
	}
	collision = false;
	return -1;
}

string Plane::showAsString(){
	string s = "PLANE:\n Point: " + this->o.showAsString()
			+ "\nnormal: " + this->normal.showAsString();
	return s;
}

void Plane::show(){
	cout << this->showAsString() << endl;
}
	
Plane Plane::operator=(Plane p){
	this->o = p.getO();
	this->normal = p.getNormal();
	this->color = p.getColor();	
	return *this;
}

Disk::Disk(){
	this->normal = Direction();
	this->o = Point();
	this->radius = -1;
	this->color = RGB();
	this->material = new Material();

}

Disk::Disk(Direction normal, Point o, float radius, RGB color){
	this->normal = normal;
	this->radius = radius;
	this->o = o;
	this->color = color;
}

Disk::Disk(Direction normal, Point o, float radius, Material* material){
	this->normal = normal;
	this->radius = radius;
	this->o = o;
	this->material = material;
}

Point Disk::getO(){
	return this->o;
}

float Disk::getRadius(){
	return this->radius;
}

Direction Disk::getNormal(){
	return this->normal;
}

RGB Disk::getColor(){
	return this->color;
}

Material* Disk::getMaterial(){
	return this->material;
}

void Disk::setMaterial(Material* material){
	this->material = material;
}

void Disk::setO(Point o){
	this->o = o;
}

void Disk::setRadius(float radius){
	this->radius = radius;
}

void Disk::setColor(RGB color){
	this->color = color;
}

void Disk::setNormal(Direction normal){
	this->normal = normal;
}

float Disk::collision(Direction d, Point o, bool& collision){
	d.normalize();
	Plane aux = Plane(this->normal, this->o, this->color);
	float dist = aux.collision(d, o, collision);
	if(collision && dist >= 0){
		Point intersection = o + d * dist;
		if(abs((intersection - this->o).modulus()) <= this->radius){
			collision = true;
			return dist;
		}
		else{
			collision = false;
			return -1;
		}
	}
	else{
		collision = false;
		return -1;
	}
}

InfiniteCylinder::InfiniteCylinder(){
	this->v = Direction();
	this->p = Point();
	this->radius = -1;
	this->color = RGB();
	this->material = new Material();
}

InfiniteCylinder::InfiniteCylinder(Direction v, Point p, float radius, RGB color){
	this->v = v;
	this->v.normalize();
	this->p = p;
	this->radius = radius;
	this->color = color;
}

InfiniteCylinder::InfiniteCylinder(Direction v, Point p, float radius, Material* material){
	this->v = v;
	this->v.normalize();
	this->p = p;
	this->radius = radius;
	this->material = material;
}

Direction InfiniteCylinder::getDirection(){
	return this->v;
}

Point InfiniteCylinder::getPoint(){
	return this->p;
}

float InfiniteCylinder::getRadius(){
	return this->radius;
}

RGB InfiniteCylinder::getColor(){
	return this->color;
}

Material* InfiniteCylinder::getMaterial(){
	return this->material;
}

void InfiniteCylinder::setMaterial(Material* material){
	this->material = material;
}

void InfiniteCylinder::setDirection(Direction v){
	this->v = v;
	this->v.normalize();
}

void InfiniteCylinder::setPoint(Point p){
	this->p = p;
}

void InfiniteCylinder::setRadius(float radius){
	this->radius = radius;
}

void InfiniteCylinder::setColor(RGB color){
	this->color = color;
}

Direction InfiniteCylinder::getNormal(Point x){
	Direction aux = x - p;
	aux.normalize();
	aux = ((v ^ aux) ^ v);
	aux.normalize();
	return aux;
}

float InfiniteCylinder::collision(Direction d, Point o, bool& collision){
	float t0, t1,t;
	d.normalize();
	collision = true;
	Direction iP = o - this->p;
    float a = (d - (this->v * (d * this->v))) * (d - (this->v * (d * this->v))); 
    float b = 2 * ((d - this->v * (d * this->v)) * (iP - (this->v * (iP * this->v))));
    float c = (iP - (this->v * (iP * this->v))) * (iP - (this->v * (iP * this->v))) 
    			- this->radius * this->radius; 
    if (!solveQuadratic(a, b, c, t0, t1)){
    	collision = false;
    	return -1;
    }
    if (t0 > t1) std::swap(t0, t1);
    if (t0 < 0) { 
        t0 = t1; 
        if (t0 < 0){
        	collision = false;
        	return -1;
        }  
    }
    t = t0; 
	return t;
}

Cylinder::Cylinder(){
	this->v = Direction();
	this->p = Point();
	this->radius = -1;
	this->color = RGB();
	this->sup = Plane();
	this->inf = Plane();
	this->material = new Material();
}

Cylinder::Cylinder(Plane inf, float h, float radius, Direction v, Point p, RGB color){
	this->v = inf.getNormal();
	this->p = inf.getO();
	this->radius = radius;
	this->color = color;
	this->inf = inf;
	sup = Plane(inf.getNormal() * -1, inf.getO() + inf.getNormal() * h,
			inf.getColor());
}

Cylinder::Cylinder(Plane inf, Plane sup, float radius, Direction v, Point p, RGB color){
	this->v = inf.getNormal();
	this->v.normalize();
	this->p = inf.getO();
	this->radius = radius;
	this->color = color;
	this->inf = inf;
	this->sup = sup; 
}

Cylinder::Cylinder(Disk bot, Disk top, RGB color){
	this->bot = bot;
	this->top = top;
	this->v = bot.getNormal();
	this->v.normalize();
	this->p = bot.getO();
	this->color = color;
	this->radius = top.getRadius();
} 

Cylinder::Cylinder(Disk bot, Disk top, Material* material){
	this->bot = bot;
	this->top = top;
	this->v = bot.getNormal();
	this->v.normalize();
	this->p = bot.getO();
	this->material = material;
	this->radius = top.getRadius();
}

Direction Cylinder::getDirection(){
	return this->v;
}

Point Cylinder::getPoint(){
	return this->p;
}

float Cylinder::getRadius(){
	return this->radius;
}

RGB Cylinder::getColor(){
	return this->color;
}

Material* Cylinder::getMaterial(){
	return this->material;
}

void Cylinder::setMaterial(Material* material){
	this->material = material;
}

Plane Cylinder::getSup(){
	return this->sup;
}

Plane Cylinder::getInf(){
	return this->inf;
}

void Cylinder::setSup(Plane sup){
	this->sup = sup;
}

void Cylinder::setInf(Plane inf){
	this->inf = inf;
}

void Cylinder::setDirection(Direction v){
	this->v = v;
	this->v.normalize();
}

void Cylinder::setPoint(Point p){
	this->p = p;
}

void Cylinder::setRadius(float radius){
	this->radius = radius;
}

void Cylinder::setColor(RGB color){
	this->color = color;
}

float Cylinder::collision(Direction d, Point o, bool& collision){
	float dist, distCyl, distTop, distBot;
	d.normalize();	
	bool colCyl, colTop, colBot;
	InfiniteCylinder aux = InfiniteCylinder(this->v, this->p, this->radius, this->color);
	distCyl = aux.collision(d, o, colCyl);
	distTop = this->top.collision(d, o, colTop);
	distBot = this->bot.collision(d, o, colBot);
	if(!colCyl && !colTop && !colBot){
		collision = false;
		return -1;
	}
	else if(!colCyl && !colTop && colBot){
		collision = false;
		return -1;
	}
	else if(!colCyl && colTop && !colBot){
		collision = false;
		return -1;
	}
	else if(!colCyl && colTop && colBot){
		if(distTop < distBot){
			collision = true;
			return distTop;
		}
		else if(distBot <= distTop){
			collision = true;
			return distBot;
		}
		else{
			collision = false;
			return -1;
		}
	}
	else if(colCyl && !colTop && !colBot){
		float dist2Bot = ((o + d * distCyl) - bot.getO()).modulus();
		float dist2Top = ((o + d * distCyl) - top.getO()).modulus();
		if(dist2Bot <= dist2Top){
			if(sqrt(dist2Top * dist2Top - this->radius * this->radius) <= (this->top.getO() - this->bot.getO()).modulus()){
				collision = true;
				return distCyl;
			}
			else{
				collision = false;
				return -1;
			}
		}
		else{
			if(sqrt(dist2Bot * dist2Bot - this->radius * this->radius) <= (this->top.getO() - this->bot.getO()).modulus()){
				collision = true;
				return distCyl;
			}
			else{
				collision = false;
				return -1;
			}
		}
	}
	else if(colCyl && !colTop && colBot){
		if(distBot <= distCyl){
			collision = true;
			return distBot;
		}
		else{
			float dist2Top = ((o + d * distCyl) - top.getO()).modulus();
			if(sqrt(dist2Top * dist2Top - this->radius * this->radius) <= (this->top.getO() - this->bot.getO()).modulus()){
				collision = true; 
				return distCyl;
			}
			else{
				collision = true;
				return distBot;
			}
		}
	}
	else if(colCyl && colTop && !colBot){
		float dist2Bot = ((o + d * distCyl) - bot.getO()).modulus();
		if(sqrt(dist2Bot * dist2Bot - this->radius * this->radius) <= (this->top.getO() - this->bot.getO()).modulus()){
			collision = true;
			if(distTop < distCyl){
				return distTop;
			}
			else{
				return distCyl;
			}
		}
		else{
			collision = true;
			return distTop;
		}
	}
	else if(colCyl && colTop && colBot){
		if(distTop < distBot){
			collision = true;
			return distTop;
		}
		else if(distBot <= distTop){
			collision = true;
			return distBot;
		}
		else{
			collision = false;
			return -1;
		}
	}
	else{
		collision = false;
		return -1;
	}
}

Triangle::Triangle() {
	this->a = Point();
	this->b = Point();
	this->c = Point();
	this->p = Plane();
	this->color = RGB();
	this->material = new Material();
}

Triangle::Triangle(Point a, Point b, Point c, RGB color) {
	this->a = a;
	this->b = b;
	this->c = c;
	Direction ab = a - b;
	Direction ac = a - c;
	Direction normal = ab ^ ac;
	normal.normalize();
	this->p = Plane(normal, this->a, this->color);
	this->color = color;
}

Triangle::Triangle(Point a, Point b, Point c, Plane p, RGB color) {
	this->a = a;
	this->b = b;
	this->c = c;
	this->p = p;
	this->color = color;
}

Triangle::Triangle(Point a, Point b, Point c, Plane p, Material* material) {
	this->a = a;
	this->b = b;
	this->c = c;
	this->p = p;
	this->material = material;
}


void Triangle::setA(Point a){
	this->a = a;
}

void Triangle::setB(Point b){
	this->b = b;
}

void Triangle::setC(Point c){
	this->c = c;
}

void Triangle::setP(Plane p){
	this->p = p;
}

void Triangle::setColor(RGB color){
	this->color = color;
}

RGB Triangle::getColor(){
	return this->color;
}

Material* Triangle::getMaterial(){
	return this->material;
}

void Triangle::setMaterial(Material* material){
	this->material = material;
}

Point Triangle::getA(){
	return this->a;
}

Point Triangle::getB(){
	return this->b;
}

Point Triangle::getC(){
	return this->c;
}

Plane Triangle::getP(){
	return this->p;
}

Direction Triangle::getNormal(Point x){
	return this->p.getNormal(x);
}

float Triangle::collision(Direction d, Point o, bool& collision){
	float dist=this->p.collision(d,o,collision);
	if(dist > 0){
		//cout << " colisiona con el plano" << endl;
		Direction edge0 = this->b - this->a; 
		Direction edge1 = this->c - this->b; 
		Direction edge2 = this->a - this->c; 
		Direction C0 = (o+d*dist) - this->a; 
		Direction C1 = (o+d*dist) - this->b; 
		Direction C2 = (o+d*dist) - this->c; 
		if (p.getNormal()*(edge0^C0) > 0 && p.getNormal()*(edge1^C1) > 0 && p.getNormal()*(edge2^C2) > 0){
			collision = true;
			return dist;
		}
		else{
			collision = false;
			return -1;
		}
	}
	else{
		collision = false;
		return -1;
	}
}
	
string Triangle::showAsString(){
	string s = "TRIANGLE:\n Point A: " + this->a.showAsString()
			+ "\nPoint B: " + this->b.showAsString()
			+ "\nPoint C: " + this->c.showAsString();
	return s;
}

void Triangle::show(){
	cout << this->showAsString() << endl;
}

Triangle Triangle::operator=(Triangle t){
	this->a = t.getA();
	this->b = t.getB();
	this->c = t.getC();
	this->color = t.getColor();	
	return *this;
}

bool solveQuadratic(float a, float b, float c, float& t0, float& t1){
	float discr = b * b - 4 * a * c; //discriminant
	if(discr < 0){ //doesnt have solution
		return false;
	}
	else if(discr == 0){//one solution
		t0 = -b / (2 * a);
		t1 = t0;
	}
	else{//two solutions
		t0 = (-b + sqrt(discr)) / (2 * a);
		t1 = (-b - sqrt(discr)) / (2 * a); 
	}
	if(t0 > t1){
		swap(t0, t1);
	}
	return true;
>>>>>>> Stashed changes
}