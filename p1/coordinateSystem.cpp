#include <iostream>
#include <cstring>
#include "coordinateSystem.h"

using namespace std;

CoordinateSystem::CoordinateSystem(){
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			this->matrix[i][j] = 0;
		}
	}
}

CoordinateSystem::CoordinateSystem(array <array<float, 4>, 4> matrix){
	this->matrix = matrix;
}

void CoordinateSystem::setI(Direction i){
	this->matrix[0][0] = i.getX();
	this->matrix[1][0] = i.getY();
	this->matrix[2][0] = i.getZ();
	this->matrix[3][0] = 0;
}

void CoordinateSystem::setJ(Direction j){
	this->matrix[0][1] = j.getX();
	this->matrix[1][1] = j.getY();
	this->matrix[2][1] = j.getZ();
	this->matrix[3][1] = 0;
}

void CoordinateSystem::setK(Direction k){
	this->matrix[0][2] = k.getX();
	this->matrix[1][2] = k.getY();
	this->matrix[2][2] = k.getZ();
	this->matrix[3][2] = 0;
}

void CoordinateSystem::setO(Point o){
	this->matrix[0][3] = o.getX();
	this->matrix[1][3] = o.getY();
	this->matrix[2][3] = o.getZ();
	this->matrix[3][3] = 1;
}

void CoordinateSystem::setMatrix(array <array<float, 4>, 4> matrix){
	this->matrix = matrix;
}

Direction CoordinateSystem::getI(){
	return Direction(this->matrix[0][0], this->matrix[0][1], this->matrix[0][2]);
}

Direction CoordinateSystem::getJ(){
	return Direction(this->matrix[1][0], this->matrix[1][1], this->matrix[1][2]);
}

Direction CoordinateSystem::getK(){
	return Direction(this->matrix[2][0], this->matrix[2][1], this->matrix[2][2]);
}

Point CoordinateSystem::getO(){
	return Point(this->matrix[3][0], this->matrix[3][1], this->matrix[3][2]);
}

array <array<float, 4>, 4> CoordinateSystem::getMatrix(){
	return this->matrix;
}

string CoordinateSystem::showAsString(){
	string s;
	for(int i = 0; i < 4; i++){
		s += "{ ";
		for(int j = 0; j < 4; j++){
			s += to_string(this->matrix[i][j]) + " ";
		}
		s += "}\n";
	}
	return s;
}

void CoordinateSystem::show(){
	cout << this->showAsString() << endl;
}

CoordinateSystem CoordinateSystem::operator=(CoordinateSystem c){
	this->matrix = c.getMatrix();
	return *this;
}

array <array<float, 4>, 4> CoordinateSystem::invert(bool& hasInverse){
    array <array<float, 4>, 4> inv;
    float det;

    inv[0][0] = matrix[1][1]  * matrix[2][2] * matrix[3][3] - 
             matrix[1][1]  * matrix[2][3] * matrix[3][2] - 
             matrix[2][1]  * matrix[1][2]  * matrix[3][3] + 
             matrix[2][1]  * matrix[1][3]  * matrix[3][2] +
             matrix[3][1] * matrix[1][2]  * matrix[2][3] - 
             matrix[3][1] * matrix[1][3]  * matrix[2][2];

    inv[1][0] = -matrix[1][0]  * matrix[2][2] * matrix[3][3] + 
              matrix[1][0]  * matrix[2][3] * matrix[3][2] + 
              matrix[2][0]  * matrix[1][2]  * matrix[3][3] - 
              matrix[2][0]  * matrix[1][3]  * matrix[3][2] - 
              matrix[3][0] * matrix[1][2]  * matrix[2][3] + 
              matrix[3][0] * matrix[1][3]  * matrix[2][2];

    inv[2][0] = matrix[1][0]  * matrix[2][1] * matrix[3][3] - 
             matrix[1][0]  * matrix[2][3] * matrix[3][1] - 
             matrix[2][0]  * matrix[1][1] * matrix[3][3] + 
             matrix[2][0]  * matrix[1][3] * matrix[3][1] + 
             matrix[3][0] * matrix[1][1] * matrix[2][3] - 
             matrix[3][0] * matrix[1][3] * matrix[2][1];

    inv[3][0] = -matrix[1][0]  * matrix[2][1] * matrix[3][2] + 
               matrix[1][0]  * matrix[2][2] * matrix[3][1] +
               matrix[2][0]  * matrix[1][1] * matrix[3][2] - 
               matrix[2][0]  * matrix[1][2] * matrix[3][1] - 
               matrix[3][0] * matrix[1][1] * matrix[2][2] + 
               matrix[3][0] * matrix[1][2] * matrix[2][1];

    inv[0][1] = -matrix[0][1]  * matrix[2][2] * matrix[3][3] + 
              matrix[0][1]  * matrix[2][3] * matrix[3][2] + 
              matrix[2][1]  * matrix[0][2] * matrix[3][3] - 
              matrix[2][1]  * matrix[0][3] * matrix[3][2] - 
              matrix[3][1] * matrix[0][2] * matrix[2][3] + 
              matrix[3][1] * matrix[0][3] * matrix[2][2];

    inv[1][1] = matrix[0][0]  * matrix[2][2] * matrix[3][3] - 
             matrix[0][0]  * matrix[2][3] * matrix[3][2] - 
             matrix[2][0]  * matrix[0][2] * matrix[3][3] + 
             matrix[2][0]  * matrix[0][3] * matrix[3][2] + 
             matrix[3][0] * matrix[0][2] * matrix[2][3] - 
             matrix[3][0] * matrix[0][3] * matrix[2][2];

    inv[2][1] = -matrix[0][0]  * matrix[2][1] * matrix[3][3] + 
              matrix[0][0]  * matrix[2][3] * matrix[3][1] + 
              matrix[2][0]  * matrix[0][1] * matrix[3][3] - 
              matrix[2][0]  * matrix[0][3] * matrix[3][1] - 
              matrix[3][0] * matrix[0][1] * matrix[2][3] + 
              matrix[3][0] * matrix[0][3] * matrix[2][1];

    inv[3][1] = matrix[0][0]  * matrix[2][1] * matrix[3][2] - 
              matrix[0][0]  * matrix[2][2] * matrix[3][1] - 
              matrix[2][0]  * matrix[0][1] * matrix[3][2] + 
              matrix[2][0]  * matrix[0][2] * matrix[3][1] + 
              matrix[3][0] * matrix[0][1] * matrix[2][2] - 
              matrix[3][0] * matrix[0][2] * matrix[2][1];

    inv[0][2] = matrix[0][1]  * matrix[1][2] * matrix[3][3] - 
             matrix[0][1]  * matrix[1][3] * matrix[3][2] - 
             matrix[1][1]  * matrix[0][2] * matrix[3][3] + 
             matrix[1][1]  * matrix[0][3] * matrix[3][2] + 
             matrix[3][1] * matrix[0][2] * matrix[1][3] - 
             matrix[3][1] * matrix[0][3] * matrix[1][2];

    inv[1][2] = -matrix[0][0]  * matrix[1][2] * matrix[3][3] + 
              matrix[0][0]  * matrix[1][3] * matrix[3][2] + 
              matrix[1][0]  * matrix[0][2] * matrix[3][3] - 
              matrix[1][0]  * matrix[0][3] * matrix[3][2] - 
              matrix[3][0] * matrix[0][2] * matrix[1][3] + 
              matrix[3][0] * matrix[0][3] * matrix[1][2];

    inv[2][2] = matrix[0][0]  * matrix[1][1] * matrix[3][3] - 
              matrix[0][0]  * matrix[1][3] * matrix[3][1] - 
              matrix[1][0]  * matrix[0][1] * matrix[3][3] + 
              matrix[1][0]  * matrix[0][3] * matrix[3][1] + 
              matrix[3][0] * matrix[0][1] * matrix[1][3] - 
              matrix[3][0] * matrix[0][3] * matrix[1][1];

    inv[3][2] = -matrix[0][0]  * matrix[1][1] * matrix[3][2] + 
               matrix[0][0]  * matrix[1][2] * matrix[3][1] + 
               matrix[1][0]  * matrix[0][1] * matrix[3][2] - 
               matrix[1][0]  * matrix[0][2] * matrix[3][1] - 
               matrix[3][0] * matrix[0][1] * matrix[1][2] + 
               matrix[3][0] * matrix[0][2] * matrix[1][1];

    inv[0][3] = -matrix[0][1] * matrix[1][2] * matrix[2][3] + 
              matrix[0][1] * matrix[1][3] * matrix[2][2] + 
              matrix[1][1] * matrix[0][2] * matrix[2][3] - 
              matrix[1][1] * matrix[0][3] * matrix[2][2] - 
              matrix[2][1] * matrix[0][2] * matrix[1][3] + 
              matrix[2][1] * matrix[0][3] * matrix[1][2];

    inv[1][3] = matrix[0][0] * matrix[1][2] * matrix[2][3] - 
             matrix[0][0] * matrix[1][3] * matrix[2][2] - 
             matrix[1][0] * matrix[0][2] * matrix[2][3] + 
             matrix[1][0] * matrix[0][3] * matrix[2][2] + 
             matrix[2][0] * matrix[0][2] * matrix[1][3] - 
             matrix[2][0] * matrix[0][3] * matrix[1][2];

    inv[2][3] = -matrix[0][0] * matrix[1][1] * matrix[2][3] + 
               matrix[0][0] * matrix[1][3] * matrix[2][1] + 
               matrix[1][0] * matrix[0][1] * matrix[2][3] - 
               matrix[1][0] * matrix[0][3] * matrix[2][1] - 
               matrix[2][0] * matrix[0][1] * matrix[1][3] + 
               matrix[2][0] * matrix[0][3] * matrix[1][1];

    inv[3][3] = matrix[0][0] * matrix[1][1] * matrix[2][2] - 
              matrix[0][0] * matrix[1][2] * matrix[2][1] - 
              matrix[1][0] * matrix[0][1] * matrix[2][2] + 
              matrix[1][0] * matrix[0][2] * matrix[2][1] + 
              matrix[2][0] * matrix[0][1] * matrix[1][2] - 
              matrix[2][0] * matrix[0][2] * matrix[1][1];

    det = matrix[0][0] * inv[0][0] + matrix[0][1] * inv[1][0] 
    	+ matrix[0][2] * inv[2][0] + matrix[0][3] * inv[3][0];

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

Point CoordinateSystem::operator*(Point p){
	array <float, 4> aux, result;
	aux[0] = p.getX();
	aux[1] = p.getY();
	aux[2] = p.getZ();
	aux[3] = 1;
	for(int i = 0; i < 4; i++){
		result[i] = aux[0] * matrix[i][0] + 
					aux[1] * matrix[i][1] + 
					aux[2] * matrix[i][2] + 
					aux[3] * matrix[i][3];
	}
	return Point(result[0], result[1], result[2]);
}