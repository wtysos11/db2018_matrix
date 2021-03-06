#include "../headers/Table.h"
#include "../headers/Heap.h"
#include "../headers/SparseMatrix.h"
#include <assert.h>
#include <cmath>
#include <queue>
#include <utility>
#include <vector>
#include <functional>
#include <ctime>
#include "../fileIO.h"
#include <cstdlib>

#include <algorithm>
#include <iostream>
#include <fstream>
using namespace std;

#define RECORD_NUM 60000
#define K_NUM 10
#define TARGET_RECORD_NUM 20

#define NUM_N 60000
#define NUM_D 784
#define PI 3.1415926

using namespace MinHeap;

double getDistance(const Matrix<float>* t_a, const Matrix<float>* t_b) {
    double res = 0;
    // the fist element of matrix is id

    for(int i = 0; i < DIMENSION; i++) {
        double tmp = t_a->getElement(0, i) - t_b->getElement(0, i);
        res += tmp * tmp;
    }

    return sqrt(res);
}

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

struct cmp1{
   bool operator() (const Node x,const Node y)
   {
       return x>y;
   }
};

/*AP函数，对给定的查询和*/
//接收一个完成knn查询的堆，一个query数组和数组的长度n。注：query数组中存放的是knn的id
//PS:不能够在Kheap进行pop操作之后再进行，结果不会正确
double AP(KHeap kheap,int* query,int n)
{
    if(n==0)
        return 0;
    double sum = 0;
    double correct = 0;//正确的knn
    double total = 0;//总数
    for(int i=0;i<n;i++)
    {
        total++;
        if(kheap.checkElement(query[i]))
        {
            correct++;
            sum += correct/total;
            cout<<query[i]<<" correct:"<<correct<<" div total:"<<total<<endl;
        }
    }
    return sum/n;
}

void knn(Matrix<float>** source,Matrix<float>* aim)
{
    clock_t clockBegin,clockEnd;
    clockBegin = clock();
    KHeap h(10,true);
    priority_queue<Node,vector<Node>,cmp1> answer;
    for(int i=1;i<NUM_N;i++)
    {
        Matrix<float>* mat = source[i];
        Node ele(mat->getId(),countingEuclidDist(mat,aim));
        h.insert(ele);
        answer.push(ele);
    }
    clockEnd = clock();

    cout<<"print"<<endl;
    h.printAll();
    cout<<"rank"<<endl;
    h.printAll2();
    cout<<"priority answer"<<endl;
    for(int i=0;i<10;i++)
    {
        cout<<"i "<<answer.top().id<<" "<<answer.top().dist<<endl;
        answer.pop();
    }



    printf("spend %d ms\n",clockEnd-clockBegin);
}

//创建一个m*d的高斯矩阵

Matrix<float>* createGaussMatrix(int m,int d)
{
    float* store = new float[m*d];
    for(int i=0;i<m*d;i++)
    {
        float z;
        do
        {
            float u1 = rand()/float(RAND_MAX);
            float u2 = rand()/float(RAND_MAX);
            z = sqrt(-2 * log(u1))*cos(2*PI*u2);
            store[i]=z;
        }while(isnan(z)||isinf(z));
    }
    Matrix<float>* gauss = new Matrix<float>(store,m,d);
    delete[] store;
    return gauss;
}

/*
m*d大小的果蝇哈希投影矩阵，有p概率为1
*/
SparseMatrix* createFlyMatrix(int m,int d,double p)
{
    int y=(int)(p*d);
    int* store = new int[m*y];
    for(int i=0;i<m;i++)
    {
        int* cache = new int[d];
        for(int j=0;j<d;j++)
        {
            cache[j]=j;
        }

        for(int j=d;j>y;j--)
        {
            int coordinate = rand()%d;
            if(cache[coordinate]!=-1)
            {
                cache[coordinate]=-1;
            }
            else
            {
                j++;
            }
        }

        int coordinate = 0;
        for(int j=0;j<d;j++)
        {
            if(cache[j]==j)
            {
                int coor = Matrix<int>::changer(i,coordinate,m,y);
                store[coor]=j;
                coordinate++;
            }
        }

        delete[] cache;
    }
    SparseMatrix* ans = new SparseMatrix(store,m,y);
    delete[] store;
    return ans;
}

/*
接收一个向量数组origin和元素数量number，已知原有维数n，要求新向量维数k，返回number个新的k维向量
向量产生方式为随机选取k个下标。
*/
Matrix<float>** disposeRandom(Matrix<float>** origin,int number,int n,int k)
{
    int* nodeList = new int[k];
    int* cache = new int[n];
    for(int i=0;i<n;i++)
    {
        cache[i] = i;
    }
    for(int i=n;i>k;i--)
    {
        int coordinate = rand()%n;
        if(cache[coordinate]!=-1)
        {
            cache[coordinate]=-1;
        }
        else
        {
            i++;
        }
    }
    int counting = 0;
    for(int i=0;i<n;i++)
    {
        if(cache[i]==i)
        {
            nodeList[counting] = i;
            counting++;
        }
    }

    Matrix<float>** ans = new Matrix<float>*[number];
    for(int i=0;i<number;i++)
    {
        ans[i] = origin[i]->createRandomDispose(nodeList,k);
    }

    delete[] nodeList;
    delete[] cache;
    return ans;
}

/*
winner take all,只保留最大的前k个分量
数组origin，数量为number，原有维度n，现在要保留k个分量
*/
Matrix<float>** disposeWTA(Matrix<float>** origin,int number,int n,int k)
{
    Matrix<float>** newone = new Matrix<float>*[number];
    for(int i=0;i<number;i++)
    {
        newone[i] = new Matrix<float>(origin[i]);
        KHeap heap(k,false);//使用最大堆
        for(int j=0;j<n;j++)
        {
            Node node(j,newone[i]->getElement(0,j));
            heap.insert(node);
        }

        for(int j=0;j<n;j++)
        {
           if(!heap.checkElement(j))
           {
               newone[i]->setElement(0,j,0);
           }
        }
    }
    return newone;
}

/*
将WTA分量化为0和1两种值
数组origin，数量为number，原有维度n，现在要保留k个分量
*/
Matrix<float>** disposeBinary(Matrix<float>** origin,int number,int n,int k)
{
    Matrix<float>** newone = new Matrix<float>*[number];
    for(int i=0;i<number;i++)
    {
        newone[i] = new Matrix<float>(origin[i]);
        int counting = 0;

        for(int j=0;j<n;j++)
        {
           if(newone[i]->getElement(0,j)!=0)
           {
               newone[i]->setElement(0,j,1);
               counting++;
           }

           if(counting == k)
               break;
        }
    }
    return newone;
}

//debug
void projectionTest(Matrix<float>** matrix)
{
    cout<<"test begin:"<<endl;
    matrix[0]->printAll();
    matrix[1]->printAll();
    matrix[NUM_N-1]->printAll();
}

//测试标准函数，接收一个矩阵数组，返回矩阵数组对于高斯
//m为原始向量长度，d为一次转换后的向量长度，k为对果蝇矩阵投影后向量继续处理的向量长度
void calculateProjection(Matrix<float>** matrix,int m,int d,int k)
{
    cout<<"After reading"<<endl;
    //很多函数要用到的随机数种子，请务必加上
    srand((unsigned)time(NULL));

    //产生高斯投影矩阵和果蝇投影矩阵
    Matrix<float>* gaussMatrix = createGaussMatrix(d,m);
    SparseMatrix* flyMatrix = createFlyMatrix(d,m,0.1);

//向量的高斯投影
    Matrix<float>** gaussProjection = new Matrix<float>*[NUM_N];
    for(unsigned int i=0;i<NUM_N;i++)
    {
        gaussProjection[i] = gaussMatrix->createProjection(matrix[i]);
    }
    cout<<"finish gauss projection"<<endl;

    //向量的果蝇投影
    Matrix<float>** flyProjection = new Matrix<float>*[NUM_N];
    for(int i=0;i<NUM_N;i++)
    {
        flyProjection[i] = flyMatrix->createProjectionFloat(matrix[i]);
    }
    cout<<"finish fly projection"<<endl;

    Matrix<float>** randomDispose = disposeRandom(flyProjection,NUM_N,d,k);
    Matrix<float>** wtaDispose = disposeWTA(flyProjection,NUM_N,d,k);
    Matrix<float>** binaryDispose = disposeBinary(wtaDispose,NUM_N,d,k);
    cout<<"origin"<<endl;
projectionTest(matrix);
cout<<"Gauss Projection"<<endl;
projectionTest(gaussProjection);
cout<<"Fly Projection"<<endl;
projectionTest(flyProjection);
cout<<"Fly Projection dispose random"<<endl;
projectionTest(randomDispose);
cout<<"Fly Projection dispose wta"<<endl;
projectionTest(wtaDispose);
cout<<"Fly Projection dispose binary"<<endl;
projectionTest(binaryDispose);
//knn测试群
cout<<"Test for knn!"<<endl;
    int aimId = NUM_N-1;//knn测试重点
    cout<<"test origin knn"<<endl;
    knn(matrix, matrix[aimId]);
    cout<<"test gauss knn"<<endl;
knn(gaussProjection, gaussProjection[aimId]);
cout<<"test flyProjection knn"<<endl;
knn(flyProjection, flyProjection[aimId]);
cout<<"test randomDispose knn"<<endl;
knn(randomDispose, randomDispose[aimId]);
cout<<"test wtaDispose knn"<<endl;
knn(wtaDispose, wtaDispose[aimId]);
cout<<"test binaryDispose knn"<<endl;
knn(binaryDispose, binaryDispose[aimId]);

for(int i=0;i<NUM_N;i++)
{
    delete flyProjection[i];
    delete gaussProjection[i];
    delete randomDispose[i];
    delete wtaDispose[i];
    delete binaryDispose[i];
}
    delete[] flyProjection;
    delete[] gaussProjection;
    delete[] randomDispose;
    delete[] wtaDispose;
    delete[] binaryDispose;
}

int main(void)
{


    Matrix<float>** matrix = fileIO("mnist",NUM_N,NUM_D);
    Matrix<float>* test = matrix[0];
    Matrix<float>* last = matrix[NUM_N-1];
    calculateProjection(matrix,NUM_D,100,16);
    //knn(matrix,test);


for(int i=0;i<NUM_N;i++)
{
    delete matrix[i];
}
    delete[] matrix;

    return 0;
}
/*
int main() {
    Table* my_table = new Table();
    MinHeap::KHeap* my_min_heap = new MinHeap::KHeap(K_NUM);

    // this should be copy and will never be changed
    // 这个还有一点bug，不过不影响现阶段的使用
    // 如果只是遍历读取记录的话，应该是没问题的

    // to do:
    // getRecord(int) 应该返回一个对象，不该返回一个指针，指针指向的
    Matrix<float>* target_matrix = new Matrix<float>(*(my_table->getRecord(TARGET_RECORD_NUM)->matrix));

    priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> my_que;

    for(int i = 0; i < RECORD_NUM; i++) {

        Record* cur_record = my_table->getRecord(i);

        int id = cur_record->matrix->getId();

        double dist = getDistance(cur_record->matrix, target_matrix);
        if(i == TARGET_RECORD_NUM) {
            cout << "target  id : " << id << " distance : " << dist << endl;
        }
        my_min_heap->insert(MinHeap::Node(id, dist));
        my_que.push(make_pair(dist, id));
    }

    for(int i = 0; i < K_NUM; i++) {
        pair<double, int> cur = my_que.top();
        my_que.pop();
        cout << " id: " << cur.second << " distance: " << cur.first << endl;
    }

    my_min_heap->printAll();
}*/
