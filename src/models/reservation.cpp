// reservation.cpp
#include "../../include/models/reservation.h"

Reservation::Reservation(int id, int customerId, int tableId, const std::string& time)
    : id(id), customerId(customerId), tableId(tableId), time(time) {}

int Reservation::getId() const {
    return id;
}

int Reservation::getCustomerId() const {
    return customerId;
}

int Reservation::getTableId() const {
    return tableId;
}

std::string Reservation::getTime() const {
    return time;
}