// manager.cpp
#include "../../include/models/manager.h"

Manager::Manager(int id, const std::string& name, double salary, int startHour, int endHour)
    : Employee(id, name, salary, startHour, endHour) {}