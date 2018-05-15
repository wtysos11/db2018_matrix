#include <iostream>
#include <string>
#include <stdio.h>
#include "matrix.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define DIMENSION 784
#define RECORDS_PER_PAGE 20
#define FRAME_NUM 50
#define PAGE_SIZE 65536
#define DATESET_DIR "mnist"
#define WASTE_SIZE 2652
#define PAGES_PER_FILE 50
#define RECORDS_PER_FILE (PAGES_PER_FILE * RECORDS_PER_FILE) 

using namespace std;

struct Record {
    Record(float* row) {
        matrix = new Matrix<float>(row, 1, DIMENSION);
    }

    ~Record() {
        if(matrix != NULL) {
            delete matrix;
        }
    }

    void print() {
        matrix->printAll();
    }
    // float id;
    // float martrix_row[DIMENSION];
    Matrix<float>* matrix;
};

class Frame {
private:
    int page_index;
    float slot_state[RECORDS_PER_PAGE];    
    float valid_slot_num;
    Record* records[RECORDS_PER_PAGE];
public:
    // default frame constructor
    Frame() {
        // do nothing
    }
    
    int getPageIndex() {
        return page_index;
    }

    // build a file from point->buf point 
    Frame(char* buf, int page_index) {
        char* start = buf;

        this->page_index = page_index;
        float* row = (float*)malloc(sizeof(float) * DIMENSION + 1);

        for(int i = 0; i < RECORDS_PER_PAGE; i++) {
            memcpy((void*)(row), (void*)buf, sizeof(float));
            buf = buf + sizeof(float);
            // printf("id = %f \n", row[0]);
            for(int j = 0; j < DIMENSION; j++) {
                memcpy((void*)(row + (j + 1)), buf, sizeof(float));
                buf = buf + sizeof(float);    
            }

            records[i] = new Record(row);
        }

        if(row != NULL) {
            free(row);
        }

        // change the file point
        buf = buf + WASTE_SIZE;

        for(int i = RECORDS_PER_PAGE - 1; i >= 0; i--) {
            memcpy(&slot_state[i], buf, sizeof(float));             
            buf = buf + sizeof(float);
        }

        memcpy(&valid_slot_num, buf, sizeof(float));             
        buf = buf + sizeof(float);
        
        assert(buf - start == PAGE_SIZE);
    }


    Record* getRecord(int id) {
        assert((id / RECORDS_PER_PAGE) == page_index);
        int index = id % RECORDS_PER_PAGE; 
        return slot_state[index] == 0 ? NULL : records[index];
    }

    // all the record in the frame should write back to file, if having any change
    void writeBack() {
        // todo 
    }
    // delete all the 
    ~Frame() {
        for(int i = 0; i < RECORDS_PER_PAGE; i++) {
            if(records[i] != NULL) delete records[i];
        }
    }
};   

// manage the frame in int the main memory
class Pager {
private:
    Frame* frames[FRAME_NUM];
    // void handleOnePage()
public:
    // init all page from file
    // write back to disk
    Pager() {
        // load first 20 pages into frames
        for(int i = 0; i < FRAME_NUM; i++) {
            load(i, i);
        }
    }

    Record* getRecord(int id) {
        int page_index = id / RECORDS_PER_PAGE;
        // int page_offset = id % RECORDS_PER_FILE % RECORDS_PER_PAGE;
        int target_frame = -1;
        do {
            // check whether current frame satisfy the condition
            for(int i = 0; i < FRAME_NUM; i++) {
                if(frames[i]->getPageIndex() == page_index) {
                    target_frame = i;
                    break;
                }
            }
            if(target_frame == -1) {
                updateFrames();
            } else {
                break;
            }
        } while(1);
        
        return frames[target_frame]->getRecord(id);           
    }

    // stage one:
    // load page from file
    // replace all the frame with the next 50 pages
    // to do: 
    // using more intelligent algorithm to update the frame
    void updateFrames() {
        int next_20_pages = frames[FRAME_NUM - 1]->getPageIndex();
        for(int i = 0; i < FRAME_NUM; i++) {
            load(next_20_pages + i + 1, i);
        }
    }
    
    // load the target page to the specific frame
    void load(int page_index, int frame_index) {
        int page_offset = (page_index % PAGES_PER_FILE) * PAGE_SIZE;
        char file_name[20];
        sprintf(file_name, "./old/%d", page_index / PAGES_PER_FILE);

        printf("src file name : %s\n", file_name);

        FILE* file_in = fopen(file_name, "rb");
        fseek(file_in, page_offset ,SEEK_SET);
        char* p_page = (char*)malloc(PAGE_SIZE);

        fread((void*)p_page, PAGE_SIZE, 1, file_in);
        if(frames[frame_index] != NULL) {
           frames[frame_index]->writeBack();
           free(frames[frame_index]); 
        }
        
        frames[frame_index] = new Frame(p_page, page_index);
        
        if(p_page != NULL) {
            free(p_page);
        }
    }

    ~Pager() {
        for(int i = 0; i < FRAME_NUM; i++) {
            if(frames[i] != NULL) {
                free(frames[i]);
            }
        }
    }
};

class Table {
private:
    Pager* pager;
public:

    Table() {
        // init
        pager = new Pager(); 
    }
    // get a pointer to the record according to the id
    // NULL -> no a record with this id
    Record* getRecord(int id) {
        return pager->getRecord(id);
    }

    Record* updateRecord(int id, Matrix<float> new_record) {
        // todo
    }

    ~Table() {
        if(pager != NULL) {
            delete pager;
        }
    }

};

int main() {
    Table* my_table = new Table();
    Record* rec = my_table->getRecord(1000);
    assert(rec != NULL);
    rec->print();
}