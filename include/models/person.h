// person.h
#ifndef PERSON_H
#define PERSON_H

#include <string>

class Person {
protected:
    int id;
    std::string name;

public:
    Person(int id, const std::string& name);
    virtual ~Person() = default;

    int getId() const;
    std::string getName() const;
    void setName(const std::string& name);
};

#endif // PERSON_H