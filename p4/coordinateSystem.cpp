#include <iostream>
#include <cstring>
#include "coordinateSystem.h"

using namespace std;

using Matrix = std::array<std::array<float, 4>, 4>;
using Vector = std::array<float, 4>;

CoordinateSystem::CoordinateSystem(){
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			this->M[i][j] = 0;
		}
	}
}

CoordinateSystem::CoordinateSystem(Direction x, Direction y, Direction z, Point o){
	for(int i = 0; i < 3; i++){
		M[i][0] = x[i];
		M[i][1] = y[i];
		M[i][2] = z[i];
		M[i][3] = o[i];
		M[3][i] = 0;
	}
	M[3][3] = 1;
}

Matrix& CoordinateSystem::getM(){
	return M;
}

void CoordinateSystem::setI(Direction i){
	this->M[0][0] = i.getX();
	this->M[1][0] = i.getY();
	this->M[2][0] = i.getZ();
	this->M[3][0] = 0;
}

void CoordinateSystem::setJ(Direction j){
	this->M[0][1] = j.getX();
	this->M[1][1] = j.getY();
	this->M[2][1] = j.getZ();
	this->M[3][1] = 0;
}

void CoordinateSystem::setK(Direction k){
	this->M[0][2] = k.getX();
	this->M[1][2] = k.getY();
	this->M[2][2] = k.getZ();
	this->M[3][2] = 0;
}

void CoordinateSystem::setO(Point o){
	this->M[0][3] = o.getX();
	this->M[1][3] = o.getY();
	this->M[2][3] = o.getZ();
	this->M[3][3] = 1;
}

Direction CoordinateSystem::getI(){
	return Direction(this->M[0][0], this->M[0][1], this->M[0][2]);
}

Direction CoordinateSystem::getJ(){
	return Direction(this->M[1][0], this->M[1][1], this->M[1][2]);
}

Direction CoordinateSystem::getK(){
	return Direction(this->M[2][0], this->M[2][1], this->M[2][2]);
}

Point CoordinateSystem::getO(){
	return Point(this->M[3][0], this->M[3][1], this->M[3][2]);
}

Vector operator*(const Matrix &a, const Vector &b){
	Vector c;
	for(int i = 0; i < 4; i++){
		c[i] = a[i][0] * b[0] + a[i][1] * b[1]
			   + a[i][2] * b[2] + a[i][3] * b[3];
	}
	return c;
}

string CoordinateSystem::showAsString(){
	string s = "COORDINATESYSTEM\n";
	for(int i = 0; i < 4; i++){
		s += "{ ";
		for(int j = 0; j < 4; j++){
			s += to_string(this->M[i][j]) + " ";
		}
		s += "}\n";
	}
	return s;
}

void CoordinateSystem::show(){
	cout << this->showAsString() << endl;
}

CoordinateSystem CoordinateSystem::operator=(CoordinateSystem c){
	this->M = c.getM();
	return *this;
}

/*dividir por vw cada coordenada??*/
Point CoordinateSystem::operator*(Point p){
	float vw =  p.getX() * this->M[3][0] + p.getY() * this->M[3][1] 
				 + p.getZ() * this->M[3][2] + 1 * this->M[3][3];
	return Point((p.getX() * this->M[0][0] + p.getY() * this->M[0][1] 
				 + p.getZ() * this->M[0][2] + 1 * this->M[0][3]) / vw,
				 (p.getX() * this->M[1][0] + p.getY() * this->M[1][1] 
				 + p.getZ() * this->M[1][2] + 1 * this->M[1][3]) / vw,
				 (p.getX() * this->M[2][0] + p.getY() * this->M[2][1] 
				 + p.getZ() * this->M[2][2] + 1 * this->M[2][3]) / vw);
}

Direction CoordinateSystem::operator*(Direction d){
	return Direction((d.getX() * this->M[0][0] + d.getY() * this->M[0][1] 
				 + d.getZ() * this->M[0][2]),
				 (d.getX() * this->M[1][0] + d.getY() * this->M[1][1] 
				 + d.getZ() * this->M[1][2]),
				 (d.getX() * this->M[2][0] + d.getY() * this->M[2][1] 
				 + d.getZ() * this->M[2][2]));
}

array <array<float, 4>, 4> CoordinateSystem::invert(bool& hasInverse){
    array <array<float, 4>, 4> inv;
    float det;

    inv[0][0] = M[1][1]  * M[2][2] * M[3][3] - 
             M[1][1]  * M[2][3] * M[3][2] - 
             M[2][1]  * M[1][2]  * M[3][3] + 
             M[2][1]  * M[1][3]  * M[3][2] +
             M[3][1] * M[1][2]  * M[2][3] - 
             M[3][1] * M[1][3]  * M[2][2];

    inv[1][0] = -M[1][0]  * M[2][2] * M[3][3] + 
              M[1][0]  * M[2][3] * M[3][2] + 
              M[2][0]  * M[1][2]  * M[3][3] - 
              M[2][0]  * M[1][3]  * M[3][2] - 
              M[3][0] * M[1][2]  * M[2][3] + 
              M[3][0] * M[1][3]  * M[2][2];

    inv[2][0] = M[1][0]  * M[2][1] * M[3][3] - 
             M[1][0]  * M[2][3] * M[3][1] - 
             M[2][0]  * M[1][1] * M[3][3] + 
             M[2][0]  * M[1][3] * M[3][1] + 
             M[3][0] * M[1][1] * M[2][3] - 
             M[3][0] * M[1][3] * M[2][1];

    inv[3][0] = -M[1][0]  * M[2][1] * M[3][2] + 
               M[1][0]  * M[2][2] * M[3][1] +
               M[2][0]  * M[1][1] * M[3][2] - 
               M[2][0]  * M[1][2] * M[3][1] - 
               M[3][0] * M[1][1] * M[2][2] + 
               M[3][0] * M[1][2] * M[2][1];

    inv[0][1] = -M[0][1]  * M[2][2] * M[3][3] + 
              M[0][1]  * M[2][3] * M[3][2] + 
              M[2][1]  * M[0][2] * M[3][3] - 
              M[2][1]  * M[0][3] * M[3][2] - 
              M[3][1] * M[0][2] * M[2][3] + 
              M[3][1] * M[0][3] * M[2][2];

    inv[1][1] = M[0][0]  * M[2][2] * M[3][3] - 
             M[0][0]  * M[2][3] * M[3][2] - 
             M[2][0]  * M[0][2] * M[3][3] + 
             M[2][0]  * M[0][3] * M[3][2] + 
             M[3][0] * M[0][2] * M[2][3] - 
             M[3][0] * M[0][3] * M[2][2];

    inv[2][1] = -M[0][0]  * M[2][1] * M[3][3] + 
              M[0][0]  * M[2][3] * M[3][1] + 
              M[2][0]  * M[0][1] * M[3][3] - 
              M[2][0]  * M[0][3] * M[3][1] - 
              M[3][0] * M[0][1] * M[2][3] + 
              M[3][0] * M[0][3] * M[2][1];

    inv[3][1] = M[0][0]  * M[2][1] * M[3][2] - 
              M[0][0]  * M[2][2] * M[3][1] - 
              M[2][0]  * M[0][1] * M[3][2] + 
              M[2][0]  * M[0][2] * M[3][1] + 
              M[3][0] * M[0][1] * M[2][2] - 
              M[3][0] * M[0][2] * M[2][1];

    inv[0][2] = M[0][1]  * M[1][2] * M[3][3] - 
             M[0][1]  * M[1][3] * M[3][2] - 
             M[1][1]  * M[0][2] * M[3][3] + 
             M[1][1]  * M[0][3] * M[3][2] + 
             M[3][1] * M[0][2] * M[1][3] - 
             M[3][1] * M[0][3] * M[1][2];

    inv[1][2] = -M[0][0]  * M[1][2] * M[3][3] + 
              M[0][0]  * M[1][3] * M[3][2] + 
              M[1][0]  * M[0][2] * M[3][3] - 
              M[1][0]  * M[0][3] * M[3][2] - 
              M[3][0] * M[0][2] * M[1][3] + 
              M[3][0] * M[0][3] * M[1][2];

    inv[2][2] = M[0][0]  * M[1][1] * M[3][3] - 
              M[0][0]  * M[1][3] * M[3][1] - 
              M[1][0]  * M[0][1] * M[3][3] + 
              M[1][0]  * M[0][3] * M[3][1] + 
              M[3][0] * M[0][1] * M[1][3] - 
              M[3][0] * M[0][3] * M[1][1];

    inv[3][2] = -M[0][0]  * M[1][1] * M[3][2] + 
               M[0][0]  * M[1][2] * M[3][1] + 
               M[1][0]  * M[0][1] * M[3][2] - 
               M[1][0]  * M[0][2] * M[3][1] - 
               M[3][0] * M[0][1] * M[1][2] + 
               M[3][0] * M[0][2] * M[1][1];

    inv[0][3] = -M[0][1] * M[1][2] * M[2][3] + 
              M[0][1] * M[1][3] * M[2][2] + 
              M[1][1] * M[0][2] * M[2][3] - 
              M[1][1] * M[0][3] * M[2][2] - 
              M[2][1] * M[0][2] * M[1][3] + 
              M[2][1] * M[0][3] * M[1][2];

    inv[1][3] = M[0][0] * M[1][2] * M[2][3] - 
             M[0][0] * M[1][3] * M[2][2] - 
             M[1][0] * M[0][2] * M[2][3] + 
             M[1][0] * M[0][3] * M[2][2] + 
             M[2][0] * M[0][2] * M[1][3] - 
             M[2][0] * M[0][3] * M[1][2];

    inv[2][3] = -M[0][0] * M[1][1] * M[2][3] + 
               M[0][0] * M[1][3] * M[2][1] + 
               M[1][0] * M[0][1] * M[2][3] - 
               M[1][0] * M[0][3] * M[2][1] - 
               M[2][0] * M[0][1] * M[1][3] + 
               M[2][0] * M[0][3] * M[1][1];

    inv[3][3] = M[0][0] * M[1][1] * M[2][2] - 
              M[0][0] * M[1][2] * M[2][1] - 
              M[1][0] * M[0][1] * M[2][2] + 
              M[1][0] * M[0][2] * M[2][1] + 
              M[2][0] * M[0][1] * M[1][2] - 
              M[2][0] * M[0][2] * M[1][1];

    det = M[0][0] * inv[0][0] + M[0][1] * inv[1][0] 
    	+ M[0][2] * inv[2][0] + M[0][3] * inv[3][0];

    if (det == 0){
       hasInverse = false;
    }
    else{
    	det = 1.0 / det;
    	hasInverse = true;
    }
    if(hasInverse){
	    for (int i = 0; i < 4; i++){
	    	for(int j = 0; j < 4; j++){
	    		inv[i][j] = inv[i][j] * det;
	    	}
	    }
	}
    return inv;
}
