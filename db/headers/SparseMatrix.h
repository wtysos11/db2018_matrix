#ifndef SparseMatrix_H
#define SparseMatrix_H

#include "matrix.h"
/*
泛型类的元素是非0元素的行下标
比如第一行的第一个元素为2，意味原矩阵第一行的第二个元素为1
*/

class SparseMatrix:public Matrix<int>
{
private:

public:
    SparseMatrix(const SparseMatrix& other)
	{
        this->m = other.m;
        this->d = other.d;
        this->id = other.id;
        int** pointer = other.element;

        //insert new one
        this->element = new int*[this->m];
        for(int i=0;i<this->m;i++)
        {
            int* rows = new int[this->d];
            for(int j=0;j<this->d;j++)
            {
                rows[j] = pointer[i][j];
            }
            this->element[i]=rows;
        }
	}
	SparseMatrix(int** pointer,int m,int d):Matrix<int>(pointer,m,d)
	{
		;
	}
	SparseMatrix(int* pointer,int m,int d):Matrix<int>(pointer,m,d)
	{
		;
	}

	Matrix<float>* createProjectionFloat(Matrix<float>* vec)
	{
	    float* ans = new float[this->m];
	    for(int i=0;i<this->m;i++)
        {
            ans[i] = 0;
            for(int j=0;j<this->d;j++)
            {
                int coor = this->element[i][j];
                ans[i] += vec->getElement(0,coor);//非零项
            }
        }

        Matrix<float>* mat = new Matrix<float>(ans,1,this->m);
        mat->setId(vec->getId());
        delete[] ans;
        return mat;
	}
	Matrix<double>* createProjectionDouble(Matrix<double>* vec)
	{
	    double* ans = new double[this->m];
	    for(int i=0;i<this->m;i++)
        {
            ans[i] = 0;
            for(int j=0;j<this->d;j++)
            {
                int coor = this->element[i][j];
                ans[i] += vec->getElement(0,coor);//非零项
            }
        }

        Matrix<double>* mat = new Matrix<double>(ans,1,this->m);
        mat->setId(vec->getId());
        delete[] ans;
        return mat;
	}
};
#endif // SparseMatrix_H
