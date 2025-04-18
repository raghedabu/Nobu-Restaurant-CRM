// food_item.cpp
#include "../../include/models/food_item.h"

FoodItem::FoodItem(int id, const std::string& name, double price)
    : id(id), name(name), price(price) {}

int FoodItem::getId() const {
    return id;
}

std::string FoodItem::getName() const {
    return name;
}

double FoodItem::getPrice() const {
    return price;
}

void FoodItem::setName(const std::string& name) {
    this->name = name;
}

void FoodItem::setPrice(double price) {
    this->price = price;
}