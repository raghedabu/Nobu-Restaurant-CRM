// waiter.cpp
#include "../../include/models/waiter.h"
#include <algorithm>

Waiter::Waiter(int id, const std::string& name, double salary, int startHour, int endHour)
    : Employee(id, name, salary, startHour, endHour) {}

const std::vector<int>& Waiter::getAssignedTables() const {
    return assignedTables;
}

void Waiter::assignTable(int tableId) {
    if (!isTableAssigned(tableId)) {
        assignedTables.push_back(tableId);
    }
}

void Waiter::removeTable(int tableId) {
    assignedTables.erase(
        std::remove(assignedTables.begin(), assignedTables.end(), tableId),
        assignedTables.end()
    );
}

bool Waiter::isTableAssigned(int tableId) const {
    return std::find(assignedTables.begin(), assignedTables.end(), tableId) != assignedTables.end();
}

void Waiter::clearTables() {
    assignedTables.clear();
}