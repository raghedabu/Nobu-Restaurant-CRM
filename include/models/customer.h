// customer.h
#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "person.h"

class Customer : public Person {
public:
    Customer(int id, const std::string& name);
    ~Customer() = default;
};

#endif // CUSTOMER_H