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
