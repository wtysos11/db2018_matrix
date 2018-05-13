#ifndef MATRIX_H
#define MATRIX_H
#include<algorithm>
#include<memory>
#include<cstdio>
/*
basic data structure.
Use row's dependency list to store
*/
//开头为0时适用，开头为1时注意减1
int changer(int x,int y,int m,int d)
{
    return x*d+y;
}

template<typename T>
class Matrix
{
private:
    T** element;
    int m;
    int d;
    int id;
public:

    Matrix<T>()
    {
        element=nullptr;
        m=0;
        d=0;
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
        this->m=other.m;
        this->d=other.d;
        T** pointer = other.element;

        //delete old one
        for(int i=0;i<m;i++)
        {
            delete[] element[i];
        }
        delete[] element;

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
    int getRow()
    {
        return m;
    }

    int getCol()
    {
        return d;
    }
    void setId(int id)
    {
        this->id = id;
    }
    int getId()
    {
        return id;
    }
    T getElement(int x,int y)
    {
        return element[x][y];
    }
    void printAll()
    {
        for(int i=0;i<m;i++)
        {
            for(int j=0;j<d;j++)
            {
                printf("%f ",element[i][j]);
            }
            printf("\n");
        }
    }
};

#endif // MATRIX_H
