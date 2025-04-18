// db_manager.h
#ifndef DB_MANAGER_H
#define DB_MANAGER_H

#include <string>
#include <vector>
#include <C:/dev-libraries/SQLiteCpp/sqlite3/sqlite3.h>
#include <memory>

#include "../models/customer.h"
#include "../models/waiter.h"
#include "../models/manager.h"
#include "../models/ceo.h"
#include "../models/food_item.h"
#include "../models/table.h"
#include "../models/reservation.h"
#include "../models/order.h"

class DBManager {
private:
    std::string dbPath;

    // Helper method to execute SQL statements
    bool executeSQL(const std::string& sql);

public:
    sqlite3* db; // Made public for simplicity

public:
    DBManager(const std::string& dbPath);
    ~DBManager();

    bool initialize();

    // Customer methods
    bool addCustomer(const Customer& customer);
    std::shared_ptr<Customer> getCustomer(int id);
    std::vector<std::shared_ptr<Customer>> getAllCustomers();

    // Employee methods
    bool addWaiter(const Waiter& waiter);
    std::shared_ptr<Waiter> getWaiter(int id);
    std::vector<std::shared_ptr<Waiter>> getAllWaiters();
    bool updateWaiterShift(int id, int startHour, int endHour);
    bool updateWaiterSalary(int id, double salary);
    bool updateWaiterTable(int waiterId, int tableId);

    bool addManager(const Manager& manager);
    std::shared_ptr<Manager> getManager(int id);
    std::vector<std::shared_ptr<Manager>> getAllManagers();
    bool updateManagerShift(int id, int startHour, int endHour);
    bool updateManagerSalary(int id, double salary);

    bool addCEO(const CEO& ceo);
    std::shared_ptr<CEO> getCEO(int id);

    // Food item methods
    bool addFoodItem(const FoodItem& foodItem);
    bool updateFoodItem(const FoodItem& foodItem);
    bool deleteFoodItem(int id);
    std::shared_ptr<FoodItem> getFoodItem(int id);
    std::vector<std::shared_ptr<FoodItem>> getAllFoodItems();

    // Table methods
    bool addTable(const Table& table);
    std::shared_ptr<Table> getTable(int id);
    std::vector<std::shared_ptr<Table>> getAllTables();
    std::vector<std::shared_ptr<Table>> getAvailableTables();
    bool updateTableStatus(int id, bool isReserved);
    bool assignWaiterToTable(int tableId, int waiterId);

    // Reservation methods
    bool addReservation(const Reservation& reservation);
    std::shared_ptr<Reservation> getReservation(int id);
    std::vector<std::shared_ptr<Reservation>> getReservationsByCustomer(int customerId);
    std::vector<std::shared_ptr<Reservation>> getReservationsByTable(int tableId);

    // Order methods
    bool addOrder(const Order& order);
    bool addOrderItem(int orderId, int foodId, int quantity);
    std::shared_ptr<Order> getOrder(int id);
    std::vector<std::shared_ptr<Order>> getOrdersByTable(int tableId);
    bool updateOrderPayment(int id, bool isPaid);

    // Method to get the next available ID from a table
    int getNextId(const std::string& tableName) const;  // <-- Add this line for getNextId declaration
};

#endif // DB_MANAGER_H
