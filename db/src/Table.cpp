#include "../headers/Table.h"

Table::Table() {
    // init
    pager = new Pager(); 
}

// get a pointer to the record according to the id
// NULL -> no a record with this id
Record* Table::getRecord(int id) {
    return pager->getRecord(id);
}
Record* updateRecord(int id, Matrix<float> new_record) {
    // todo
}

Table::~Table() {
    if(pager != NULL) {
        delete pager;
    }
}