#include "../headers/Table.h"
#include "../headers/Heap.h"
#include <assert.h>
#include <math.h>
#include <queue>
#include <utility>
#include <vector>
#include <functional>

#define RECORD_NUM 60000
#define K_NUM 10
#define TARGET_RECORD_NUM 20
// 
double getDistance(const Matrix<float>* t_a, const Matrix<float>* t_b) {
    double res = 0;
    // the fist element of matrix is id

    for(int i = 0; i < DIMENSION; i++) {
        double tmp = t_a->getElement(0, i) - t_b->getElement(0, i);
        res += tmp * tmp;
    }

    return sqrt(res);
}


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
}