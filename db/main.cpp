#include <iostream>
#include <string>
#include "fileIO.h"
#include "matrix.h"
#include <cmath>
#include "heap.h"
#include <time.h>
#include <algorithm>
#include <queue>

#define NUM_N 60000
#define NUM_D 784

using namespace MinHeap;
using namespace std;

struct cmp
{
     bool operator ()(Node x, Node y)
    {
        return x > y;//小的优先级高
    }
};

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
    priority_queue<Node,vector<Node>,cmp> h;
    int maximum=0;
    int k=10;
    for(int i=1;i<NUM_N;i++)
    {
        Matrix<float>* mat = source[i];
        Node ele(mat->getId(),countingEuclidDist(mat,aim));
        h.push(ele);
    }
    for(int i=0;i<k;i++)
    {
        Node head = h.top(); h.pop();
        printf("%d %f\n",head.id,head.dist);
    }
}

void knn2(Matrix<float>** source,Matrix<float>* aim)
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
    //test->printAll();
    Matrix<float>* last = matrix[n-1];
    //last->printAll();

    knn(matrix,test);
    knn2(matrix,test);
    delete[] matrix;

    return 0;
}
