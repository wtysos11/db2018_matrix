#include "../headers/Pager.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Record::Record(float* row) {
    matrix = new Matrix<float>(row, 1, DIMENSION);
}

Record::~Record() {
    if(matrix != NULL) {
        delete matrix;
    }
}

void Record::print() {
    matrix->printAll();
}

// Frame method implements

Frame::Frame() {
    // do nothing
}

int Frame::getPageIndex() {
    return page_index;
}
// build a file from point->buf point 
Frame::Frame(char* buf, int page_index) {
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

Record* Frame::getRecord(int id) {
    assert((id / RECORDS_PER_PAGE) == page_index);
    int index = id % RECORDS_PER_PAGE; 
    return slot_state[index] == 0 ? NULL : records[index];
}
// all the record in the frame should write back to file, if having any c
void Frame::writeBack() {
    // todo 
}
// delete all frames 
Frame::~Frame() {
    for(int i = 0; i < RECORDS_PER_PAGE; i++) {
        if(records[i] != NULL) delete records[i];
    }
}

// implement of pager
Pager::Pager() {
    // load first 20 pages into frames
    for(int i = 0; i < FRAME_NUM; i++) {
        load(i, i);
    }
}

Record* Pager::getRecord(int id) {
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


void Pager::load(int page_index, int frame_index) {
    int page_offset = (page_index % PAGES_PER_FILE) * PAGE_SIZE;
    char file_name[20];
    sprintf(file_name, "../old/%d", page_index / PAGES_PER_FILE);
    printf("src file name : %s\n", file_name);
    FILE* file_in = fopen(file_name, "rb");
    assert(file_in != NULL);
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

void Pager::updateFrames() {
    int next_20_pages = frames[FRAME_NUM - 1]->getPageIndex();
    for(int i = 0; i < FRAME_NUM; i++) {
        load(next_20_pages + i + 1, i);
    }
}

Pager::~Pager() {
    for(int i = 0; i < FRAME_NUM; i++) {
        if(frames[i] != NULL) {
            free(frames[i]);
        }
    }
}