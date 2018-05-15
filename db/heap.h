#ifndef HEAP_H
#define HEAP_H
#include <cstdio>
#include <iostream>
#include <fstream>
using namespace std;
//knn's counter
/*
It maintains a k min-heap and update when there's something smaller or not enough element in KHeap.
It only store the matrix's id.
*/
#define NOT -1
#define MAX 100000
#define LeftChild(x) (2*x+1)
#define RightChild(x) (2*x+2)

namespace MinHeap
{
struct Node
{
    int id;
    double dist;
    Node()
    {
        id=NOT;
        dist=NOT;
    }
    Node(int id,double dist)
    {
        this->id = id;
        this->dist = dist;
    }
    Node(const Node& other)
    {
        this->id = other.id;
        this->dist = other.dist;
    }

    bool operator<(const Node& another) const
    {
        return dist<another.dist;
    }
    bool operator>(const Node& another) const
    {
        return dist>another.dist;
    }
    bool operator==(const Node& another)const
    {
        return dist==another.dist;
    }
    bool operator!=(const Node& another) const
    {
        return dist!=another.dist;
    }
    bool operator<=(const Node& another) const
    {
        return (*this<another)||(*this==another);
    }
    Node& operator=(const Node& other)
    {
        this->id = other.id;
        this->dist = other.dist;
        return *this;
    }
};


class KHeap
{
private:
    Node* heap;//storage pointer
    int k;//number
    int index;//pointer point to the last value
    double maximum;
    int maxPosition;
public:
    KHeap(int k)
    {
        index=-1;
        this->k=k;
        heap = new Node[k];
        for(int i=0;i<k;i++)
        {
            heap[i].dist=NOT;
            heap[i].id=NOT;
        }
        maximum=0;
        maxPosition = 0;
    }
    KHeap(KHeap& another)
    {
        index = another.index;
        k = another.k;
        heap = new Node[k];
        for(int i=0;i<k;i++)
        {
            heap[i] = another.heap[i];
        }
        maximum = another.maximum;
        maxPosition = another.maxPosition;
    }
    ~KHeap()
    {
        delete[] heap;
    }

    bool insert(Node element)
    {
        if(index<k-1)
        {
            index++;
            heap[index]=element;
            if(maximum<element.dist)
            {
                maximum=element.dist;
                maxPosition=index;
            }
            MinHeapMaintain(index);
        }
        else if(index==k-1 && element.dist<maximum)//when index == k,index can't add. heap[k] is ready for cache.
        {
            int changeId = maxPosition;
            heap[maxPosition]=element;
            maximum = element.dist;
            for(int i=0;i<k;i++)
            {
                if(heap[i].dist>maximum)
                {
                    maximum = heap[i].dist;
                    maxPosition = i;
                }
            }
            //cout<<"IN element:"<<element.id<<endl;
            //this->printAll();
            MinHeapMaintain(changeId);
            //cout<<"After that"<<endl;
            //this->printAll();
        }
        else
        {
            return false;
        }


        return true;
    }

    void MinHeapMaintain(int index)
    {
        int i,j;
        Node temp = heap[index];
        i = index;
        j = (index-1)/2;
        while (j >= 0 && i != 0)
        {
            if (heap[j] <= temp)//can't swap
                break;
            //cout<<"change in ["<<j<<"] "<<heap[j].dist<<"and ["<<i<<"] "<<heap[i].dist<<endl;
            if(j == maxPosition)
            {
                maxPosition = i;
            }
            heap[i] = heap[j];     //把较大的子结点往下移动,替换它的子结点
            i = j;
            j = (i-1)/2;
        }
        heap[i] = temp;
    }

    void DownMaintain(int i)
    {
        int l = LeftChild(i), r = RightChild(i);
        int smallest = i;
        if (l < k&&heap[l] < heap[i])
            smallest = l;
        if (r < k&&heap[r] < heap[smallest])
            smallest = r;
        if (smallest != i){
            Node temp = heap[i];
            heap[i] = heap[smallest];
            heap[smallest] = temp;
            if(i == maxPosition)
            {
                maxPosition = smallest;
            }
            DownMaintain(smallest);
        }
    }

    Node pop()
    {
        if(index==-1)
        {
            return Node();
        }

        Node top = heap[0];
        //Node maxi(NOT,MAX);
        //heap[0] = maxi;
        heap[0] = heap[index];
        index--;

        if(index==-1)
        {
            return top;
        }
        else
        {
            k--;
            DownMaintain(0);
            return top;
        }
    }

    bool isEmpty()
    {
        return (index<0);
    }

    void printAll()//debug
    {
        printf("In min-heap, %d nearest neighbour\n",k);
        for(int i=0;i<k;i++)
        {
            printf("number=%d id=%d dist=%f",i,heap[i].id,heap[i].dist);
            if(LeftChild(i)<k&&heap[i]>heap[LeftChild(i)])
            {
                printf(" Left Child Problem!");
            }
            if(RightChild(i)<k&&heap[i]>heap[RightChild(i)])
            {
                printf(" Right Child Problem!");
            }
            printf("\n");
        }
    }

    void printAll2()
    {
        printf("In min-heap printAll2, %d nearest neighbour\n",k);
        int counter = 0;
        while(!isEmpty())
        {
            Node top = pop();
            printf("number=%d id=%d dist=%f\n",counter++,top.id,top.dist);
        }
    }
/*
    void printAll2()
    {
        for(int i=0;i<k-1;i++)
        {
            for(int j=0;j<k-1-i;j++)
            {
                if(heap[j]>heap[j+1])
                {
                    Node temp = heap[j];
                    heap[j] = heap[j+1];
                    heap[j+1] = temp;
                }
            }
        }
        for(int i=0;i<k;i++)
        {
            printf("number=%d id=%d dist=%f\n",i,heap[i].id,heap[i].dist);
        }
    }*/
};
}
#endif // HEAP_H
