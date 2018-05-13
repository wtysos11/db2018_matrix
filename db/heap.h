#ifndef HEAP_H
#define HEAP_H
#include <cstdio>
//knn's counter
/*
It maintains a k min-heap and update when there's something smaller or not enough element in KHeap.
It only store the matrix's id.
*/
#define NOT -1
#define LeftChild(x) (2*x+1)
#define RightChild(x) (2*x+2)
#define getParent(x) ((x-1)/2)

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
public:
    KHeap(int k)
    {
        index=-1;
        this->k=k;
        heap = new Node[k+1];
        for(int i=0;i<=k;i++)
        {
            heap[i].dist=NOT;
            heap[i].id=NOT;
        }
        maximum=0;
    }
    KHeap(KHeap& another)
    {
        index = another.index;
        k = another.k;
        heap = new Node[k+1];
        for(int i=0;i<=k;i++)
        {
            heap[i] = another.heap[i];
        }
        maximum = another.maximum;
    }
    ~KHeap()
    {
        delete[] heap;
    }

    bool insert(Node element)
    {
        if(index<k)
        {
            index++;
            heap[index]=element;
        }
        else if(index==k && element.dist<maximum)//when index == k,index can't add. heap[k] is ready for cache.
        {
            heap[index]=element;
        }
        else
        {
            return false;
        }

        if(maximum<element.dist)
            maximum=element.dist;

        MinHeapMaintain(index);
        return true;
    }

    void MinHeapMaintain(int index)
    {
        int i,j;
        Node temp = heap[index];
        i = index;
        j = getParent(index);
        while (j >= 0 && i != 0)
        {
            if (heap[j] <= temp)//can't swap
                break;

            heap[i] = heap[j];     //把较大的子结点往下移动,替换它的子结点
            i = j;
            j = (i - 1) / 2;
        }
        heap[i] = temp;
    }

    void printAll()//debug
    {
        printf("In min-heap, %d nearest neighbour\n",k);
        for(int i=0;i<=k;i++)
        {
            printf("number=%d id=%d dist=%lf\n",i,heap[i].id,heap[i].dist);
        }
    }
};
}
#endif // HEAP_H
