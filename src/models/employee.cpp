// employee.cpp
#include "../../include/models/employee.h"

Employee::Employee(int id, const std::string& name, double salary, int startHour, int endHour)
    : Person(id, name), salary(salary), startHour(startHour), endHour(endHour) {}

double Employee::getSalary() const {
    return salary;
}

void Employee::setSalary(double salary) {
    this->salary = salary;
}

int Employee::getStartHour() const {
    return startHour;
}

void Employee::setStartHour(int startHour) {
    this->startHour = startHour;
}

int Employee::getEndHour() const {
    return endHour;
}

void Employee::setEndHour(int endHour) {
    this->endHour = endHour;
}