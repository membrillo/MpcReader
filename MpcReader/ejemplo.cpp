// MpcReader.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdio>
#include "MpcReader.h"
#include <ctype.h>




int main(int argc, char* argv[]) {
	MpcReader reader("case6ww_GCEP.m"); // esto carga todos los datos de las matrices tal cual
	reader.read(); // se leen
	
	mpc mpc; // creo un objeto a mpc
	mpc = reader.getMpc(); // lo inicializo dandole que sea igual al que se obtuvo de leer el archivo


	mpc.printVariablesStored(); // se imprimen todas las variables en ese mpc

	// Despues de cargar los datos los tengo que cortar/ordenar/modificar para que queden en el formato deseado
	mpcMatrix aux;
	aux.setColumn(mpc.getMatrixVariable("branch").getCol(1), 1);
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

	// si se hace algo despues de esto conviene liberar la memoria ya que no se usaran mas estos objetos

	mpc.clearMemory();




}


