// person.cpp
#include "../../include/models/person.h"

Person::Person(int id, const std::string& name) : id(id), name(name) {}

int Person::getId() const {
    return id;
}

std::string Person::getName() const {
    return name;
}

void Person::setName(const std::string& name) {
    this->name = name;
}