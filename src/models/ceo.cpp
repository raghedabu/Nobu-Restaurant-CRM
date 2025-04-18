// ceo.cpp
#include "../../include/models/ceo.h"

CEO::CEO(int id, const std::string& name, double salary, int startHour, int endHour)
    : Employee(id, name, salary, startHour, endHour) {}