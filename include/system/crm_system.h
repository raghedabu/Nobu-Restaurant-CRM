// crm_system.h
#ifndef CRM_SYSTEM_H
#define CRM_SYSTEM_H

#include <string>
#include <vector>
#include <memory>

#include "../database/db_manager.h"
#include "../models/customer.h"
#include "../models/waiter.h"
#include "../models/manager.h"
#include "../models/ceo.h"
#include "../models/food_item.h"
#include "../models/table.h"
#include "../models/reservation.h"
#include "../models/order.h"

// Role enumeration for access control
enum class Role {
    CUSTOMER,
    WAITER,
    MANAGER,
    CEO
};

class CRMSystem {
private:
    std::shared_ptr<DBManager> dbManager;
    Role currentRole;
    int currentUserId;

public:
    CRMSystem(const std::string& dbPath);
    ~CRMSystem() = default;

    // Access to DBManager for initialization
    std::shared_ptr<DBManager> getDbManager() const { return dbManager; }
    
    // Initialization
    bool initializeSystem();
    
    // Login/authentication
    bool login(Role role, int userId);
    void logout();
    Role getCurrentRole() const;
    int getCurrentUserId() const;
    
    // Customer functionality
    bool registerCustomer(const std::string& name);
    bool makeReservation(int tableId, const std::string& time);
    std::vector<std::shared_ptr<Table>> getAvailableTables() const;
    
    // Waiter functionality
    bool viewOrdersForTable(int tableId, std::vector<std::shared_ptr<Order>>& orders) const;
    
    // Manager functionality
    bool addWaiter(const std::string& name, double salary, int startHour, int endHour);
    bool updateWaiterShift(int waiterId, int startHour, int endHour);
    bool updateWaiterSalary(int waiterId, double salary);
    bool assignTableToWaiter(int tableId, int waiterId);
    
    bool addFoodItem(const std::string& name, double price);
    bool updateFoodItem(int id, const std::string& name, double price);
    bool removeFoodItem(int id);
    
    // CEO functionality
    bool addManager(const std::string& name, double salary, int startHour, int endHour);
    bool updateManagerShift(int managerId, int startHour, int endHour);
    bool updateManagerSalary(int managerId, double salary);
    
    // Common functionality
    std::vector<std::shared_ptr<FoodItem>> getMenu() const;
    bool placeOrder(int tableId, const std::vector<std::pair<int, int>>& foodItems); // Pairs of food ID and quantity
    
    // System setup
    bool setupTables();
    
    // Helper methods
    int getNextId(const std::string& tableName) const;
};

#endif // CRM_SYSTEM_H