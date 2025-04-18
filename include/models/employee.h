// employee.h
#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include "person.h"

class Employee : public Person {
protected:
    double salary;
    int startHour;
    int endHour;

public:
    Employee(int id, const std::string& name, double salary, int startHour, int endHour);
    virtual ~Employee() = default;

    double getSalary() const;
    void setSalary(double salary);
    
    int getStartHour() const;
    void setStartHour(int startHour);
    
    int getEndHour() const;
    void setEndHour(int endHour);
};

#endif // EMPLOYEE_H