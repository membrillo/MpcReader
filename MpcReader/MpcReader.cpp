// MpcReader.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdio>
#include "mpc.h"
#include <ctype.h>
#include "parser.h"

//using namespace std;


const int NONE = 0;
const int VARIABLE_DECLARATION = 1;
const int MATRIX_DECLARATION = 2;
const int MATRIX_END_DECLARATION = 3;
const int MATRIX_CONTINUE = 4;
const int MATRIX_SINGLE_LINE = 5;
const int VECTOR_DECLARATION = 6;
const int ALPHA_DECLARATION = 7;

std::size_t matrixDeclarationFound;
std::size_t equalFound;
std::size_t matrixEndFound;
std::size_t endOfLineFound;
std::size_t startParenthesisFound;
std::size_t endParenthesisFound;

std::vector <std::string> variableNames;
std::vector <std::size_t> variableFounds;


bool matrixFound=false;




void split(std::string word, std::string delimiter){
	std::size_t found = word.find(delimiter);
	int startPosition;
	int endPosition;
	if (found != std::string::npos){
		std::cout << word << std::endl;
	}
		
}


std::string getName(std::string line){

	std::string variableName;
	
	std::size_t dotfound = line.find(".");
	
	if (dotfound!=std::string::npos && dotfound<equalFound){
		variableName = line.substr(dotfound + 1, equalFound - dotfound-1);
	}
	else{

		variableName = line.substr(0,equalFound);
	}

	if (variableName.substr(variableName.length() - 1).compare(" ") == 0){
		variableName = variableName.substr(0, variableName.length() - 1 );
	}
	

	return variableName;

}


int detectTypeOfLine(std::string line){
	std::string matrixDeclarationToFind = "[";
	std::string parenthesisStartDeclarationToFind = "(";
	std::string parenthesisEndDeclarationToFind = ")";
	std::string equalDeclarationToFind = "=";
	std::string matrixEndDeclarationToFind = "]";
	std::string endOfLine = ";";

	matrixDeclarationFound = line.find(matrixDeclarationToFind);
	equalFound = line.find(equalDeclarationToFind);
	matrixEndFound = line.find(matrixEndDeclarationToFind);
	endOfLineFound = line.find(endOfLine);
	startParenthesisFound = line.find(parenthesisStartDeclarationToFind);
	endParenthesisFound = line.find(parenthesisEndDeclarationToFind);
	

	if (equalFound != std::string::npos && matrixDeclarationFound != std::string::npos && equalFound<matrixDeclarationFound && matrixEndFound==std::string::npos){ // si pilla un igual, y un [ despues de este, entonces se esta declarando una matriz
		matrixFound = true;
		return MATRIX_DECLARATION;
	}
	else if (equalFound != std::string::npos && endOfLineFound != std::string::npos && matrixEndFound == std::string::npos && matrixDeclarationFound == std::string::npos && endParenthesisFound ==std::string::npos){
		return VARIABLE_DECLARATION;
	}
	else if (matrixEndFound != std::string::npos && matrixDeclarationFound == std::string::npos){
		matrixFound = false;
		return MATRIX_END_DECLARATION;
	}
	else if(matrixFound){
		return MATRIX_CONTINUE;
	}
	else if (matrixEndFound != std::string::npos && equalFound != std::string::npos && matrixDeclarationFound != std::string::npos){
		return MATRIX_SINGLE_LINE;
	}
	else if (equalFound != std::string::npos && startParenthesisFound !=std::string::npos && endParenthesisFound !=std::string::npos && startParenthesisFound<equalFound){
		return VECTOR_DECLARATION;
	}
	else {
		return NONE;
	}




}

// le doy una linea y da el valor, solo util cuando es un valor, no una matriz ni vector
double getValueFromLine(std::string line, int startPosition){
	
	int valueStartPosition=NULL;
	int valueEndPosition=NULL;
	char firstChar;


	bool valid = true;

	for (int i = startPosition+1; i < line.length(); i++){
		char letter = line.at(i);
		std::string letterString = line.substr(i, 1);

		firstChar = NULL;
		if (!letterString.compare(" ")==0 && !letterString.compare("=")==0 && firstChar == NULL &&valid)
		{
			firstChar = letter;
			if (isalpha(letter)){
				valid = false;
				std::cout << firstChar << std::endl;
				break;
			}
			
			
		}


		if (valueStartPosition==NULL && isdigit(letter) && valid)
		{
			valueStartPosition = i;
		}
		else if (valueStartPosition != NULL && letterString.compare(";") == 0){
			valueEndPosition = i;
		}
	}

	std::string valueString=  line.substr(valueStartPosition, valueEndPosition - valueStartPosition);

	std::cout << valueString << std::endl;

	

	



	if (valid){
		double value = std::stod(valueString);
		return value;
	}
	else {
		return 0;
	}
	



	

}




//bugeado no usar
double getValueFromLine(std::string line){

	int valueStartPosition = NULL;
	int valueEndPosition = NULL;
	char firstChar;


	bool valid = true;

	for (int i = 0; i < line.length(); i++){
		char letter = line.at(i);
		std::string letterString = line.substr(i, 1);

		firstChar = NULL;
		if (!letterString.compare(" ") == 0 && !letterString.compare("=") == 0 && firstChar == NULL &&valid)
		{
			firstChar = letter;
			if (isalpha(letter)){
				valid = false;
				std::cout << firstChar << std::endl;
				break;
			}


		}


		if (valueStartPosition == NULL && isdigit(letter) && valid)
		{
			valueStartPosition = i;
		}
		else if (valueStartPosition != NULL && !isdigit(letter) && valid){
			valueEndPosition = i;
			//std::cout << firstChar << std::endl;
		}



	}


	if (valid){

		double value = std::stod(line.substr(valueStartPosition, valueEndPosition - valueStartPosition));
		return value;
	}
	else {
		return 0;
	}






} //bugeado no usar // bugeado, no usar    // bugeado no usar


bool isValidObjectDeclaration(){
	bool aux = false;
	for (int i = 0; i < variableFounds.size(); i++){
		if (variableFounds.at(i) != std::string::npos){
			aux = true;
			break;
		}

	}

	return aux;
}


int main(int argc, char* argv[]) {

	


	mpc mpc;

	
	std::vector <double> temporalRow;
	
	
	bool matrixDeclarationStarted=false;
	bool matrixDeclarationEnded = false;
	
	std::string line;
	std::string* matrixesNames = new std::string[3]{ "bus", "gen", "branch" };



	std::string lastMatrixName;

	

	std::ifstream myfile("case6ww_GCEP.m");

	

	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{

			std::cout << line << '\n';

			switch (detectTypeOfLine(line)){
			case MATRIX_DECLARATION:
			{
				temporalRow.clear();
				lastMatrixName = getName(line);
				mpc.addNewMatrix(lastMatrixName);
				int startPosition = NULL;
				for (int i = matrixDeclarationFound; i < line.length(); i++){
					std::string auxLetter = line.substr(i, 1);
					//std::cout << "[" << auxLetter << "]";

					if (isdigit(line.at(i)) && startPosition == NULL){
						startPosition = i;
						//std::cout << i;
					}
					else if ((!isdigit(line.at(i)) && !auxLetter.compare(".") == 0) && startPosition != NULL){


						temporalRow.push_back(std::stod(line.substr(startPosition, i - startPosition)));
						//std::cout << "[" << temporalRow.at(temporalRow.size() - 1) << "]";
						startPosition = NULL;
					}
				}
				
				
				//std::cout << "Matriz empieza" << std::endl;
				break;
			}

			case VARIABLE_DECLARATION:
			{
				
				mpc.addVariable(getName(line), getValueFromLine(line, equalFound));
				std::cout << "agregada variable con nombre : " << getName(line) << " y valor : " <<mpc.getVariable(getName(line)) << std::endl;
					break;
			}

			case MATRIX_END_DECLARATION:
			{
				
				std::cout << "Matriz termina" << std::endl;
				break;
			}

			case MATRIX_CONTINUE:
			{
				
				int startPosition = NULL;
				for (int i = 0; i < line.length(); i++){
					std::string auxLetter = line.substr(i, 1);
					//std::cout << "[" << auxLetter << "]";
					
					if ((isdigit(line.at(i)) || auxLetter.compare("-") == 0) && startPosition == NULL){
						startPosition = i;
						//std::cout << i;
					}
					
					else if ((!isdigit(line.at(i)) && !auxLetter.compare(".") == 0) && startPosition != NULL){
						temporalRow.push_back(std::stod(line.substr(startPosition, i - startPosition)));
						//std::cout << "["<< temporalRow.at(temporalRow.size()-1)  << "]";
						startPosition = NULL;
						if (auxLetter.compare(";") == 0){
							mpc.getMatrixVariable(lastMatrixName).addRowFromVector(temporalRow);
							//std::cout << temporalRow.size();
							temporalRow.clear();
							break;
						}

					}
				}
				
				std::cout << "Matriz " << lastMatrixName << mpc.getMatrixVariable(lastMatrixName).name <<" continua" << std::endl;
				break;
			}
			case MATRIX_SINGLE_LINE:
			{
				temporalRow.clear();
				lastMatrixName = getName(line);
				std::cout << lastMatrixName << std::endl;
				
				mpc.addNewMatrix(lastMatrixName);
				int startPosition = NULL;
				for (int i = 0; i < line.length(); i++){
					std::string auxLetter = line.substr(i, 1);
					//std::cout << "[" << auxLetter << "]";

					if ((isdigit(line.at(i)) || auxLetter.compare("-") == 0) && startPosition == NULL){
						startPosition = i;
						//std::cout << i;
					}

					else if ((!isdigit(line.at(i)) && !auxLetter.compare(".") == 0) && startPosition != NULL){
						temporalRow.push_back(std::stod(line.substr(startPosition, i - startPosition)));
						//std::cout << "["<< temporalRow.at(temporalRow.size()-1)  << "]";
						startPosition = NULL;
						if (auxLetter.compare("]") == 0){
							mpc.getMatrixVariable(lastMatrixName).addRowFromVector(temporalRow);
							//std::cout << temporalRow.size();
							temporalRow.clear();
							break;
						}

					}
				}

				
				//std::cout << "Matriz en una linea" << std::endl;
				break;

			}
			case VECTOR_DECLARATION:
			{
				if (mpc.matrixExists(getName(line))){
					mpc.addNewMatrix(getName(line));
					std::cout << "Vector creado" << std::endl;
				}
				else{
					std::cout << "Vector existente" << std::endl;
				}
				std::cout << "valor : " << getValueFromLine(line,equalFound) << std::endl;
				break;
			}

			case NONE:
			{
				std::cout << "NADA" << std::endl;
				break;
			}



			}


		}
		myfile.close();
	}
	else std::cout << "Unable to open file";
	
	
	mpc.printVariablesStored();
	
	// Despues de cargar los datos los tengo que cortar/ordenar/modificar para que queden en el formato deseado
	mpcMatrix aux;
	aux.setColumn(mpc.getMatrixVariable("branch").getCol(1),1);
	aux.setColumn(mpc.getMatrixVariable("branch").getCol(2), 2);
	aux.setColumn(mpc.getMatrixVariable("branch").getCol(3), 3);


	mpc.addNewMatrix("Line");
	mpc.getMatrixVariable("Line") = (aux);
	mpc.writeMatrix("Line");

	aux.clearMatrix();
	aux.setColumn(mpc.getMatrixVariable("bus").getCol(1), 1);
	aux.setColumn(mpc.getMatrixVariable("bus").getCol(2), 2);
	aux.setColumn(mpc.getMatrixVariable("bus").getCol(10), 5);

	mpc.getMatrixVariable("bus") = aux;
	mpc.getMatrixVariable("bus").changeName("Bus"); // Le cambio el nombre solo para ponerle la mayuscula al principio
	mpc.writeMatrix("Bus");

	aux.clearMatrix();
	aux.setColumn(mpc.getMatrixVariable("gen").getCol(1), 1);
	aux.setColumn(mpc.getMatrixVariable("gen").getCol(2), 2);
	aux.setColumn(mpc.getMatrixVariable("gen").getCol(9), 4);
	aux.setColumn(mpc.getMatrixVariable("gen").getCol(10), 5);
	aux.setColumn(mpc.getMatrixVariable("gen").getCol(11), 6);

	mpc.getMatrixVariable("gen") = aux;
	mpc.getMatrixVariable("gen").changeName("Gen"); // Le cambio el nombre solo para ponerle la mayuscula al principio
	mpc.writeMatrix("Gen");



	return 0;
}


