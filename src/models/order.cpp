// order.cpp
#include "../../include/models/order.h"

Order::Order(int id, int tableId, const std::string& time)
    : id(id), tableId(tableId), time(time), totalAmount(0.0), isPaid(false) {}

int Order::getId() const {
    return id;
}

int Order::getTableId() const {
    return tableId;
}

std::string Order::getTime() const {
    return time;
}

const std::vector<OrderItem>& Order::getItems() const {
    return items;
}

double Order::getTotalAmount() const {
    return totalAmount;
}

bool Order::getIsPaid() const {
    return isPaid;
}

void Order::addItem(int foodId, int quantity) {
    items.push_back({foodId, quantity});
}

void Order::setTotalAmount(double amount) {
    totalAmount = amount;
}

void Order::setPaid(bool isPaid) {
    this->isPaid = isPaid;
}