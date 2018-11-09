#include <iostream>
#include <cstring>
#include <fstream>
#include <vector>
#include <cmath>
#include <limits>
#include "rgb.h"
#include "xyz.h"
#include "xyy.h"

using namespace std;

long int rows, columns, c;
float  MAX = -1;
vector<vector<RGB>> matrix;
vector<vector<float>> matrixLDR;
string path = "";
float minValue = std::numeric_limits<float>::infinity(), maxValue = 0;

void fillMatrix(ifstream& f){
	string s, s2, s3;
	long int r, g, b;
	RGB rgb();
	getline(f, s);
	getline(f, s);
	while(s.at(0) == '#'){
		s2 = s.substr(1,3);
		if(s2.compare("MAX") == 0){
			MAX = stof(s.substr(5, s.size() - 5), nullptr);
		}
		getline(f, s);
	}
	rows = stol(s.substr(0, s.find(' ')));
	columns = stol(s.substr(s.find(' ') + 1, s.size() - s.find(' ') + 1));
	f >> c;
	matrix.resize(rows);
	for(int i = 0; i < rows; i++){
		matrix[i].resize(columns);	
	}
	for(int i = 0; i < rows; i++){
		for(int j = 0; j < columns; j++){
			f >> r; f >> g; f >> b;
			matrix[i][j].setR((float) (r / c));
			matrix[i][j].setG((float) (g / c));
			matrix[i][j].setB((float) (b / c));
			if(MAX != -1){
				matrix[i][j].setR(matrix[i][j].getR()*MAX);
				matrix[i][j].setG(matrix[i][j].getG()*MAX);
				matrix[i][j].setB(matrix[i][j].getB()*MAX);
			}
		}
	}
}

void showMatrix(){
	for(int i = 0; i < rows; i++){
		for(int j = 0; j < columns; j++){
			cout << matrix[i][j].getR() << " " << matrix[i][j].getG() << " " << matrix[i][j].getB() << "     ";
		}
		cout << endl;
	}
}

void clamp(){
	for(int i = 0; i < rows; i++){
		for(int j = 0; j < columns * 3; j++){
			/*if(matrixLDR[i][j] > 15000){
				matrixLDR[i][j] = 15000;
			}*/
		}
	}
}

void equalize(){

}

void saveMatrix(){
	ofstream o(path+"LDR.ppm");
	o << "P3" << endl;
	o << "#MAX=" << MAX << endl;
	string aux = path;
	aux = path.substr(aux.find("/") + 1, path.size() - aux.find("/"));
	o << "#" << aux << "LDR.ppm" << endl;
	o << rows << " " << columns << endl;
	o << "255" << endl;
	//o << c << endl;
	for(int i = 0; i < rows; i++){
		for(int j = 0; j < columns * 3; j++){
			//o << (long int) ((matrixLDR[i][j] * 255	) / 15000) << " ";
			//o << (long int) ((matrixLDR[i][j])) << " ";
		}
		o << endl;
	}
	o.close();
}

int main(){
	/*cout << "Choose tone mapper" << endl;
	cout << "Input path to image: " << flush;
	cout << endl;
	path = "../../hdr-ppm/seymour_park";
	//cin >> path;
	ifstream f;
	f.open(path+".ppm");
	if(f.is_open()){
		fillMatrix(f);
		//matrixLDR = matrix;
		clamp();
		saveMatrix();
		f.close();
	}
	else{
		cout << "ERROR: couldnt access to file " << path << endl;
	}*/
	RGB rgb(201, 102, 58);
	XYZ xyz = rgb.toXYZ();
	xyY xyy = rgb.toXYY();
	cout << "RGB\n" << " r = " << rgb.getR() << ", g = " << rgb.getG() << ", b = " << rgb.getB() << endl;
	cout << "XYZ\n" << " x = " << xyz.getX() << ", y = " << xyz.getY() << ", Z = " << xyz.getZ() << endl;
	cout << "XYY\n" << " x = " << xyy.getX() << ", y = " << xyy.getY() << ", Y = " << xyy.getYLum() << endl;
}
