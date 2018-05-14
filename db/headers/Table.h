#ifndef TABLE_H
#define TABLE_H
#include "Pager.h"

class Table {
private:
    Pager* pager;
public:
    Table();
    // get a pointer to the record according to the id
    // NULL -> no a record with this id
    Record* getRecord(int id);

    Record* updateRecord(int id, Matrix<float> new_record);

    ~Table();
};
#endif