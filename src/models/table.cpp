// table.cpp
#include "../../include/models/table.h"

Table::Table(int id) : id(id), isReserved(false), waiterId(-1) {}

int Table::getId() const {
    return id;
}

bool Table::getIsReserved() const {
    return isReserved;
}

void Table::setIsReserved(bool isReserved) {
    this->isReserved = isReserved;
}

int Table::getWaiterId() const {
    return waiterId;
}

void Table::setWaiterId(int waiterId) {
    this->waiterId = waiterId;
}