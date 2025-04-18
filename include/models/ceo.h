// ceo.h
#ifndef CEO_H
#define CEO_H

#include "employee.h"

class CEO : public Employee {
public:
    CEO(int id, const std::string& name, double salary, int startHour, int endHour);
    ~CEO() = default;
};

#endif // CEO_H