#ifndef MATRIX_H
#define MATRIX_H
#include<algorithm>
#include<memory>
#include<iostream>
#include <fstream>
using namespace std;
/*
basic data structure.
Use row's dependency list to store
*/
template<typename T>
class Matrix
{
protected:
    T** element;
    int m;
    int d;
    int id;

public:

    static int changer(int x,int y,int m,int d)
    {
        return x*d+y;
    }

    Matrix<T>()
    {
        element=nullptr;
        m=0;
        d=0;
    }

    Matrix<T>(const Matrix<T> &other) {
        this->m = other.m;
        this->d = other.d;
        this->id = other.id;
        T** pointer = other.element;

        //insert new one
        element = new T*[m];
        for(int i=0;i<m;i++)
        {
            T* rows = new T[d];
            for(int j=0;j<d;j++)
            {
                rows[j] = pointer[i][j];
            }
            element[i]=rows;
        }

    }

    Matrix<T>(int m,int d)//m rows, d cols matrix element in two-dimension
    {
        this->m=m;
        this->d=d;
        element = new T*[m];
        for(int i=0;i<m;i++)
        {
            T* rows = new T[d];
            for(int j=0;j<d;j++)
            {
                rows[j] = 0;

            }
            element[i]=rows;
        }
    }
    Matrix<T>(T** pointer,int m,int d)//m rows, d cols matrix element in two-dimension
    {
        this->m=m;
        this->d=d;
        element = new T*[m];
        for(int i=0;i<m;i++)
        {
            T* rows = new T[d];
            for(int j=0;j<d;j++)
            {
                rows[j] = pointer[i][j];

            }
            element[i]=rows;
        }
    }
    Matrix<T>(T* pointer,int m,int d)//m rows, d cols matrix element in two-dimension
    {
        this->m=m;
        this->d=d;
        element = new T*[m];
        for(int i=0;i<m;i++)
        {
            T* rows = new T[d];
            for(int j=0;j<d;j++)
            {
                rows[j] = pointer[changer(i,j,m,d)];

            }
            element[i]=rows;
        }
    }
    ~Matrix()
    {
        for(int i=0;i<m;i++)
        {
            delete[] element[i];
        }
        delete[] element;
    }



//static function
    //return a matrix(m*d) with all zeros


    //opeartor
    Matrix& operator=(const Matrix& other)
    {
        // why this function never call
        this->m=other.m;
        this->d=other.d;
        T** pointer = other.element;

        //delete old one
        if(element != nullptr) {
            for(int i=0;i<m;i++)
            {
                delete[] element[i];
            }
            delete[] element;
        }

        //insert new one
        element = new T*[m];
        for(int i=0;i<m;i++)
        {
            T* rows = new T[d];
            for(int j=0;j<d;j++)
            {
                rows[j] = pointer[i][j];
            }
            element[i]=rows;
        }
        return *this;
    }
    Matrix operator*(Matrix& other)
    {
        Matrix<T> ans(this->getRow(),other.getCol());
        for(int i=0;i<ans.getRow();i++)
        {
            for(int j=0;j<ans.getCol();j++)
            {
                //for each element, count the sum
                T** pointer = other.getElement();
                for(int k=0;k<this->getCol();k++)
                {
                    ans.element[i][j]+=this->element[i][k]*pointer[k][j];
                }


            }
        }
        return ans;
    }
    //通过高斯矩阵产生投影，这样行向量可以不用转置直接相乘
    //注：函数不检查合法性，请在调用前确保，不然可能会发生奇怪的问题。
    Matrix<T> createProjection(Matrix<T> vec)
    {
        T* ans = new T[this->m];
        for(int i=0;i<this->m;i++)
        {
            ans[i]=0;
            for(int j=0;j<this->d;j++)
            {
                ans[i]+=this->element[i][j]*vec.getElement(0,j);
            }
        }

        Matrix<T> mat(ans,1,this->m);
        delete[] ans;
        return mat;
    }

    //matrix row operation
    T row_sum(int row)
    {
        T sum=0;
        for(int i=0;i<d;i++)
        {
            sum+=element[row][i];
        }
        return sum;
    }

    double row_average(int row)
    {
        T sum = this->row_sum(row);
        return sum/d;
    }

    //add a const to a row
    void row_add(int row,double ele)
    {
        for(int j=0;j<d;j++)
        {
            element[row][j]+=ele;
        }
    }
    //plus a const to a row
    void row_plus(int row,double ele)
    {
        for(int j=0;j<d;j++)
        {
            element[row][j]*=ele;
        }
    }
    //divide a const to a row
    void row_divide(int row,double ele)
    {
        for(int j=0;j<d;j++)
        {
            element[row][j]/=ele;
        }
    }

    //matrix col operation
    //add a const to a col
    void col_add(int col,double ele)
    {
        for(int i=0;i<m;i++)
        {
            element[i][col]+=ele;
        }
    }


    //using for debug
    // should be defined as a const member function
    int getRow() const
    {
        return m;
    }

    int getCol() const
    {
        return d;
    }

    void setId(int id)
    {
        this->id = id;
    }
    int getId() const
    {
        return id;
    }
    T getElement(int x,int y) const
    {
        return element[x][y];
    }
    void printAll() const
    {
        for(int i=0;i<m;i++)
        {
            for(int j=0;j<d;j++)
            {
                cout<<element[i][j]<<" ";
            }
            cout<<endl;
        }
    }
    //高斯分布检验
    void checkGauss()
    {
        double sum=0;
        for(int i=0;i<m;i++)
        {
            double row = 0;
            for(int j=0;j<d;j++)
            {
                row+=element[i][j];
            }
        }
        sum/=(m*d);
        cout<<"Gauss average:"<<sum<<endl;
        double variance=0;
        for(int i=0;i<m;i++)
        {
            for(int j=0;j<d;j++)
            {
                variance+=(element[i][j]-sum)*(element[i][j]-sum);
            }
        }
        variance/=(m*d);
        cout<<"Gauss average:"<<variance<<endl;
    }
};

#endif
