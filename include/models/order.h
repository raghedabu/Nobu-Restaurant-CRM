// order.h
#ifndef ORDER_H
#define ORDER_H

#include <vector>
#include <string>

struct OrderItem {
    int foodId;
    int quantity;
};

class Order {
private:
    int id;
    int tableId;
    std::string time;
    std::vector<OrderItem> items;
    double totalAmount;
    bool isPaid;

public:
    Order(int id, int tableId, const std::string& time);
    ~Order() = default;

    int getId() const;
    int getTableId() const;
    std::string getTime() const;
    const std::vector<OrderItem>& getItems() const;
    double getTotalAmount() const;
    bool getIsPaid() const;

    void addItem(int foodId, int quantity);
    void setTotalAmount(double amount);
    void setPaid(bool isPaid);
};

#endif // ORDER_H