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
vector<vector<RGB> > matrix;
vector<vector<RGB> > matrixLDR;
vector<vector<xyY> > matrixXYY;
string path = "", outputPath = "";
float minValue = std::numeric_limits<float>::infinity(), maxValue = 0;
float equalizationValue = 0;

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
	matrixLDR.resize(rows);
	matrixXYY.resize(rows);
	for(int i = 0; i < rows; i++){
		matrix[i].resize(columns);
		matrixLDR[i].resize(rows);
		matrixXYY[i].resize(rows);
	}
	for(int i = 0; i < rows; i++){
		for(int j = 0; j < columns; j++){
			f >> r; f >> g; f >> b;
			matrix[i][j].setR(((float) r / c));
			matrix[i][j].setG(((float) g / c));
			matrix[i][j].setB(((float) b / c));
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

void changeToXYY(){
	for(int i = 0; i < rows; i++){
		for(int j = 0; j < columns; j++){
			matrixXYY[i][j] = matrix[i][j].toXYY(); 
			if(matrixXYY[i][j].getYLum() > equalizationValue){
				equalizationValue = matrixXYY[i][j].getYLum();
			}
		}
	}
}


void changeToRGB(){
	for(int i = 0; i < rows; i++){
		for(int j = 0; j < columns; j++){
			matrixLDR[i][j] = matrixXYY[i][j].toRGB(); 
		}
	}
}

void clamp(float clampValue){
	for(int i = 0; i < rows; i++){
		for(int j = 0; j < columns; j++){
			if(matrixXYY[i][j].getYLum() > clampValue){
				matrixXYY[i][j].setYLum(clampValue);
			}
		}
	}
}

void equalize(){
	for(int i = 0; i < rows; i++){
		for(int j = 0; j < columns; j++){
			matrixXYY[i][j].setYLum(matrixXYY[i][j].getYLum()/equalizationValue);
		}
	}
}

void gammaCurve(float a, float gamma){
	for(int i = 0; i < rows; i++){
		for(int j = 0; j < columns; j++){
			matrixXYY[i][j].setYLum(a * powf(matrixXYY[i][j].getYLum(), gamma));
		}
	}
}

void saveMatrix(){
	ofstream o(outputPath+".ppm");
	o << "P3" << endl;
	o << "#MAX=" << MAX << endl;
	string aux = path;
	aux = path.substr(aux.find("/") + 1, path.size() - aux.find("/"));
	o << "#" << aux << "LDR.ppm" << endl;
	o << rows << " " << columns << endl;
	o << "255" << endl;
	//o << c << endl;
	for(int i = 0; i < rows; i++){
		for(int j = 0; j < columns; j++){
			o << (long int) ((matrixLDR[i][j].getR())) << " ";
			o << (long int) ((matrixLDR[i][j].getG())) << " ";
			o << (long int) ((matrixLDR[i][j].getB())) << "    ";
			//o << (long int) ((matrixLDR[i][j])) << " ";
		}
		o << endl;
	}
	o.close();
}

int main(int argc, char* argv[]){
	if(argc != 3){
		cout << "ERROR: use " << argv[0] << " inputFile outputFile." << endl;
	}
	else{
		cout << "Choose tone mapper:" << endl;
		cout << "Clamping - 1, Equalization - 2, Equalize and Clamp - 3, GammaCurve - 4, Clamp and GammaCurve - 5: " << flush;
		int option;
		cin >> option;
		float clampValue, A, gamma;
		path = "../../hdr-ppm/seymour_park";
		path = argv[1]; outputPath = argv[2];
		if(option > 0 && option < 6){
			ifstream f;
			f.open(path+".ppm");
			if(f.is_open()){
				fillMatrix(f);
				matrixLDR = matrix;
				changeToXYY();
				switch(option){
				case 1:
					cout << "Select clamp value: " << flush;
					cin >> clampValue;
					clamp(clampValue);
					break;
				case 2:
					equalize();
					break;
				case 3:
					cout << "Select clamp value and equalization value: " << flush;
					cin >> clampValue >> equalizationValue;
					clamp(clampValue);
					equalize();
					break;
				case 4:
					cout << "Select value A and gamma: "  << flush;
					cin >> A >> gamma;
					gammaCurve(A, gamma);
					break;
				case 5:
					cout << "Select clamp value, A and gamma: " << flush;
					cin >> clampValue >> A >> gamma;
					clamp(clampValue);
					gammaCurve(A, gamma);
					break;
				}
				changeToRGB();
				saveMatrix();
				f.close();
			}
			else{
				cout << "ERROR: couldnt access to file " << path << endl;
			}
		}
		else{
			cout << "Invalid option." << endl;
		}
		
	}
	
}
