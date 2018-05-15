#ifndef PAGER_H
#define PAGER_H 

#include "matrix.h"

#define DIMENSION 784
#define RECORDS_PER_PAGE 20
#define FRAME_NUM 50
#define PAGE_SIZE 65536
#define DATESET_DIR "mnist"
#define WASTE_SIZE 2652
#define PAGES_PER_FILE 50
#define RECORDS_PER_FILE (PAGES_PER_FILE * RECORDS_PER_FILE) 

struct Record {
    Record(float* row, int id);

    ~Record();

    void print();

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
    Frame();
    
    int getPageIndex();

    // build a file from point->buf point 
    Frame(char* buf, int page_index);

    // get one record from the file
    Record* getRecord(int id);

    // all the record in the frame should write back to file, if having any change
    void writeBack();
    
    // free all records  
    ~Frame();
}; 

// manage the frame in int the main memory
class Pager {
private:
    Frame* frames[FRAME_NUM];
    // void handleOnePage()
public:
    // init all page from file
    // write back to disk
    Pager();

    Record* getRecord(int id);

    // stage one:
    // load page from file
    // replace all the frame with the next 50 pages
    // to do: 
    // using more intelligent algorithm to update the frame
    void updateFrames();
    
    // load the target page to the specific frame
    void load(int page_index, int frame_index);

    // free all the frame from memory
    ~Pager();
};
#endif