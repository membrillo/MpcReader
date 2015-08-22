#pragma once
#include <string>
#include <vector>

class mpcMatrix{
public:
	std::string name;
	int rows;
	int cols;
	bool isValid;
	mpcMatrix(); 
	mpcMatrix(std::string newName);
	mpcMatrix(std::string newName, int rows, int cols);
	double** matrix;
	std::vector<std::vector<double> > array2D;
	void resize(int rows, int cols);
	void addRowFromVector(std::vector <double> rowVector);
	void setColumn(std::vector <double> columnVector, int columnIndex);
	void setRow(std::vector <double> rowVector, int rowIndex);

	std::vector <double> getRow(int rowIndex);
	std::vector <double> getCol(int columnIndex);

	void mpcMatrix::operator=(mpcMatrix matrixToCopy);
	double& mpcMatrix::operator()(const int n, const int m);
	double** getAsDynamicArray();
	

	void clearMatrix();
	void printMatrix();
	void addFromString(std::string line);
	void changeName(std::string newName);
	~mpcMatrix();

};

