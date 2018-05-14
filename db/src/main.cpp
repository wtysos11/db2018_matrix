#include "../headers/Table.h"
#include <assert.h>

int main() {
    Table* my_table = new Table();
    Record* rec = my_table->getRecord(1000);
    assert(rec != NULL);
    rec->print();
}