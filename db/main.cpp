#include <iostream>
#include <string>
#include "fileIO.h"
#include "matrix.h"
#include <cmath>
#include "heap.h"
#include <time.h>

#define NUM_N 60000
#define NUM_D 784

using namespace MinHeap;


template<typename T>
double countingEuclidDist(Matrix<T>* a,Matrix<T>* b)
{
    if(a->getRow()!=1||b->getRow()!=1||a->getCol()!=b->getCol())
    {
        return -1;
    }

    double dist = 0;
    for(int i=0;i<a->getCol();i++)
    {
        if(a->getElement(0,i)==0&&b->getElement(0,i)==0)
        {
            continue;
        }
        else
        {
            dist+=(a->getElement(0,i)-b->getElement(0,i))*(a->getElement(0,i)-b->getElement(0,i));
        }
    }

    return sqrt(dist);
}

void knn(Matrix<float>** source,Matrix<float>* aim)
{
    clock_t clockBegin,clockEnd;
    clockBegin = clock();
    KHeap h(10);
    for(int i=1;i<NUM_N;i++)
    {
        Matrix<float>* mat = source[i];
        Node ele(mat->getId(),countingEuclidDist(mat,aim));
        h.insert(ele);
    }
    clockEnd = clock();
    h.printAll();
    h.printAll2();
    printf("spend %d ms\n",clockEnd-clockBegin);
}

int main(void)
{

    int n=NUM_N;
    int d=NUM_D;

    Matrix<float>** matrix = fileIO("mnist",n,d);
    Matrix<float>* test = matrix[0];
    test->printAll();
    Matrix<float>* last = matrix[n-1];
    last->printAll();

    knn(matrix,test);

    delete[] matrix;

    return 0;
}
