// manager.h
#ifndef MANAGER_H
#define MANAGER_H

#include "employee.h"

class Manager : public Employee {
public:
    Manager(int id, const std::string& name, double salary, int startHour, int endHour);
    ~Manager() = default;
};

#endif // MANAGER_H