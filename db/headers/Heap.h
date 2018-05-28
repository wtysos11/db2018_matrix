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
#define MAX (2<<30)
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
    double border;
    int borderPosition;
	bool flag;//flag为true时是最小堆，flag为flase时是最大堆
public:
    KHeap(int k,bool flag = true)
    {
        index = -1;
        this->k = k;
		this->flag = flag;
        heap = new Node[k];
        for(int i=0;i<k;i++)
        {
            heap[i].dist=NOT;
            heap[i].id=NOT;
        }
        if(flag)
            border = 0;
        else
            border = MAX;
        borderPosition = 0;
    }
    KHeap(KHeap& another)
    {
        index = another.index;
        k = another.k;
		flag = another.flag;
        heap = new Node[k];
        for(int i=0;i<k;i++)
        {
            heap[i] = another.heap[i];
        }
        border = another.border;
        borderPosition = another.borderPosition;
    }
    ~KHeap()
    {
        delete[] heap;
    }
	bool judge(int x,int y)
	{
		if(flag)//最小堆
		{
			return x<y;
		}
		else//最大堆
		{
			return x>y;
		}
	}
	bool judge(Node x,Node y)
	{
		if(flag)//最小堆
		{
			return x<y;
		}
		else//最大堆
		{
			return x>y;
		}
	}
	bool getFlag(void)
	{
	    return flag;
	}

    bool insert(Node element)
    {
        if(index<k-1)
        {
            index++;
            heap[index]=element;
			bool borderChange = false;

            if(judge(border,element.dist))
            {
                border=element.dist;
                borderPosition=index;
            }
            HeapMaintain(index);
        }
        else if(index==k-1 && judge(element.dist,border))//when index == k,index can't add. heap[k] is ready for cache.
        {
            int changeId = borderPosition;
            heap[borderPosition]=element;
            border = element.dist;
            for(int i=k/2;i<k;i++)//border must be a leaf
            {
                if(judge(border,heap[i].dist))
                {
                    border = heap[i].dist;
                    borderPosition = i;
                }
            }
            //cout<<"IN element:"<<element.id<<endl;
            //this->printAll();
            HeapMaintain(changeId);
            //cout<<"After that"<<endl;
            //this->printAll();
        }
        else
        {
            return false;
        }


        return true;
    }
    void HeapMaintain(int index)
    {
        int i,j;
        Node temp = heap[index];
        i = index;
        j = (index-1)/2;
        while (j >= 0 && i != 0)
        {
            if (judge(heap[j],temp)||heap[j]==temp)//can't swap
                break;
            //cout<<"change in ["<<j<<"] "<<heap[j].dist<<"and ["<<i<<"] "<<heap[i].dist<<endl;
            if(j == borderPosition)
            {
                borderPosition = i;
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
        int changeOne = i;
        if (l <=index&& judge(heap[l],heap[i]))
            changeOne = l;
        if (r <=index&&judge(heap[r],heap[changeOne]))
            changeOne = r;
        if (changeOne != i){
            Node temp = heap[i];
            heap[i] = heap[changeOne];
            heap[changeOne] = temp;
            if(i == borderPosition)
            {
                borderPosition = changeOne;
            }
            DownMaintain(changeOne);
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

    //find if the ordered id is in the knn
    bool checkElement(int orderId)
    {
        for(int i=0;i<k;i++)
        {
            if(heap[i].id == orderId)
                return true;
        }
        return false;
    }
    int* output(void)
    {
        int* nodeList = new int [k];
        for(int i=0;i<k;i++)
        {
            nodeList[i] = heap[i].id;
        }
        return nodeList;
    }
};
}
#endif // HEAP_H
