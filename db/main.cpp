#include <iostream>
#include <string>

using namespace std;

int main(void)
{
    int ** ary = new int*[2];
    int *x = new int[3];
    int *y = new int[4];

    x[0]=1;
    y[3]=2;
    ary[0]=x;
    ary[1]=y;



    delete[] x;
    delete[] y;
    delete[] ary;
    return 0;
}
