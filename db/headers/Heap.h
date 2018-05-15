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
        }
        else if(index==k-1 && element.dist<maximum)//when index == k,index can't add. heap[k] is ready for cache.
        {
            heap[maxPosition]=element;
        }
        else
        {
            return false;
        }

        if(maximum<element.dist)
        {
            maximum=element.dist;
            maxPosition=index;
        }

        MinHeapMaintain(index);
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

            heap[i] = heap[j];     //�ѽϴ���ӽ�������ƶ�,�滻�����ӽ��?
            i = j;
            j = (i-1)/2;
        }
        heap[i] = temp;
    }

    void DownMaintain(int index)
    {
        if(LeftChild(index)>=k)//in case out of range
            return;

        Node lchild = heap[LeftChild(index)];
        Node rchild;

        if(RightChild(index)<k)//in case out of range
             rchild= heap[RightChild(index)];

        if(lchild.id==-1)//if there's no element, return
        {
            return;
        }

        //find the smallest element, maintain the min-heap
        int smallest = index;
        if(lchild<heap[index])
            smallest = LeftChild(index);

        if(rchild.id!=-1 && rchild<heap[smallest])
        {
            smallest = RightChild(index);
        }

        //if there's change, swap the element and continue.
        if(smallest!=index)
        {
            Node swaping = heap[index];
            heap[index] = heap[smallest];
            heap[smallest] = swaping;

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
        heap[0]=heap[index];
        index--;

        if(index==-1)
        {
            return top;
        }
        else
        {
            DownMaintain(0);
            k--;
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
            printf("number=%d id=%d dist=%f\n",i,heap[i].id,heap[i].dist);
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
};
}
#endif // HEAP_H