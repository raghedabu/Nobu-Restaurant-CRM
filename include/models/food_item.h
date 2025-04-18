// food_item.h
#ifndef FOOD_ITEM_H
#define FOOD_ITEM_H

#include <string>

class FoodItem {
private:
    int id;
    std::string name;
    double price;

public:
    FoodItem(int id, const std::string& name, double price);
    ~FoodItem() = default;

    int getId() const;
    std::string getName() const;
    double getPrice() const;

    void setName(const std::string& name);
    void setPrice(double price);
};

#endif // FOOD_ITEM_H