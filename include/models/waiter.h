// waiter.h
#ifndef WAITER_H
#define WAITER_H

#include <vector>
#include "employee.h"

class Waiter : public Employee {
private:
    std::vector<int> assignedTables;

public:
    Waiter(int id, const std::string& name, double salary, int startHour, int endHour);
    ~Waiter() = default;

    const std::vector<int>& getAssignedTables() const;
    void assignTable(int tableId);
    void removeTable(int tableId);
    bool isTableAssigned(int tableId) const;
    void clearTables();
};

#endif // WAITER_H