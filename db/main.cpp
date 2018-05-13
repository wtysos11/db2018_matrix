#include <iostream>
#include <string>
#include "matrix.h"
using namespace std;
typedef int Test;
int main(void)
{
    int** test = new int*[2];
    int* x = new int[3];
    x[0]=1,x[1]=2,x[2]=3;
    int* y = new int[3];
    y[0]=4,y[1]=5,y[2]=6;
    test[0]=x;
    test[1]=y;

    Matrix<int> matrix(test,2,3);

    int ** another = new int*[3];
    for(int i=0;i<3;i++)
    {
        int* s = new int[2];
        for(int j=0;j<2;j++)
        {
            s[j]=2*i+j;
        }
        another[i]=s;
    }
    Matrix<int> other(another,3,2);

    matrix = matrix * other;

    Test** store = matrix.getElement();

    for(int i=0;i<2;i++)
    {
        for(int j=0;j<2;j++)
        {
            cout<<store[i][j]<<" ";
        }
        cout<<endl;
    }
    delete[] x;
    delete[] y;
    delete[] test;
    return 0;
}
