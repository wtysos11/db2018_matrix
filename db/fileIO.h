#ifndef FILEIO_H
#define FILEIO_H
#include <stdio.h>
#include "matrix.h"
/*
Read data from dataset and do something you like.
infile : filename of dataset
n      : number of data point to be read
d      : dimension of a data point
*/
Matrix<float>** fileIO(const char* infile, const int n, const int d) {
	FILE* fin = NULL;

	fin = fopen(infile, "r");
    Matrix<float>** matrix = new Matrix<float>*[n];
	//data is float to glove, int to mnist
	float td;
	int ti;
	float* mem = new float[d];
	for (int i = 0; i < n; i++) {

		// read id into ti
		fscanf(fin, "%d", &ti);
		// read data1 to datad
		for (int j = 0; j < d; j++) {

			// read one single data into td
			fscanf(fin, "%f", &td);
            mem[j] = td;
		}

		matrix[i] = new Matrix<float>(mem,1,d);
	}

    delete[] mem;
	fclose(fin);
	return matrix;
}
#endif // FILEIO_H
