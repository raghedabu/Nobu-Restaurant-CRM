// crm_system.cpp
#include "../../include/system/crm_system.h"
#include <iostream>
#include <ctime>
#include <iomanip>
#include <sstream>

CRMSystem::CRMSystem(const std::string& dbPath) 
    : dbManager(std::make_shared<DBManager>(dbPath)), 
      currentRole(Role::CUSTOMER), 
      currentUserId(-1) {}

bool CRMSystem::initializeSystem() {
    if (!dbManager->initialize()) {
        return false;
    }
    
    // Set up 64 tables if they don't exist yet
    return setupTables();
}

bool CRMSystem::login(Role role, int userId) {
    switch (role) {
        case Role::CUSTOMER:
            if (dbManager->getCustomer(userId)) {
                currentRole = role;
                currentUserId = userId;
                return true;
            }
            break;
        case Role::WAITER:
            if (dbManager->getWaiter(userId)) {
                currentRole = role;
                currentUserId = userId;
                return true;
            }
            break;
        case Role::MANAGER:
            if (dbManager->getManager(userId)) {
                currentRole = role;
                currentUserId = userId;
                return true;
            }
            break;
        case Role::CEO:
            if (dbManager->getCEO(userId)) {
                currentRole = role;
                currentUserId = userId;
                return true;
            }
            break;
    }
    
    return false;
}

void CRMSystem::logout() {
    currentRole = Role::CUSTOMER;
    currentUserId = -1;
}

Role CRMSystem::getCurrentRole() const {
    return currentRole;
}

int CRMSystem::getCurrentUserId() const {
    return currentUserId;
}

// Customer functionality
bool CRMSystem::registerCustomer(const std::string& name) {
    int id = getNextId("customers");
    Customer customer(id, name);
    return dbManager->addCustomer(customer);
}

bool CRMSystem::makeReservation(int tableId, const std::string& time) {
    // Only customers can make reservations
    if (currentRole != Role::CUSTOMER || currentUserId == -1) {
        return false;
    }
    
    // Check if table exists and is available
    auto table = dbManager->getTable(tableId);
    if (!table || table->getIsReserved()) {
        return false;
    }
    
    // Create reservation
    int reservationId = getNextId("reservations");
    Reservation reservation(reservationId, currentUserId, tableId, time);
    
    return dbManager->addReservation(reservation);
}

std::vector<std::shared_ptr<Table>> CRMSystem::getAvailableTables() const {
    return dbManager->getAvailableTables();
}

// Waiter functionality
bool CRMSystem::viewOrdersForTable(int tableId, std::vector<std::shared_ptr<Order>>& orders) const {
    // Only waiters can view orders, and they can only see their assigned tables
    if (currentRole != Role::WAITER || currentUserId == -1) {
        return false;
    }
    
    auto waiter = dbManager->getWaiter(currentUserId);
    if (!waiter || !waiter->isTableAssigned(tableId)) {
        return false;
    }
    
    orders = dbManager->getOrdersByTable(tableId);
    return true;
}

// Manager functionality
bool CRMSystem::addWaiter(const std::string& name, double salary, int startHour, int endHour) {
    // Only managers can add waiters
    if (currentRole != Role::MANAGER || currentUserId == -1) {
        return false;
    }
    
    int id = getNextId("waiters");
    Waiter waiter(id, name, salary, startHour, endHour);
    return dbManager->addWaiter(waiter);
}

bool CRMSystem::updateWaiterShift(int waiterId, int startHour, int endHour) {
    // Only managers can update waiter shifts
    if (currentRole != Role::MANAGER || currentUserId == -1) {
        return false;
    }
    
    return dbManager->updateWaiterShift(waiterId, startHour, endHour);
}

bool CRMSystem::updateWaiterSalary(int waiterId, double salary) {
    // Only managers can update waiter salaries
    if (currentRole != Role::MANAGER || currentUserId == -1) {
        return false;
    }
    
    return dbManager->updateWaiterSalary(waiterId, salary);
}

bool CRMSystem::assignTableToWaiter(int tableId, int waiterId) {
    // Only managers can assign tables to waiters
    if (currentRole != Role::MANAGER || currentUserId == -1) {
        return false;
    }
    
    return dbManager->assignWaiterToTable(tableId, waiterId);
}

bool CRMSystem::addFoodItem(const std::string& name, double price) {
    // Only managers can add food items
    if (currentRole != Role::MANAGER || currentUserId == -1) {
        return false;
    }
    
    int id = getNextId("food_items");
    FoodItem foodItem(id, name, price);
    return dbManager->addFoodItem(foodItem);
}

bool CRMSystem::updateFoodItem(int id, const std::string& name, double price) {
    // Only managers can update food items
    if (currentRole != Role::MANAGER || currentUserId == -1) {
        return false;
    }
    
    FoodItem foodItem(id, name, price);
    return dbManager->updateFoodItem(foodItem);
}

bool CRMSystem::removeFoodItem(int id) {
    // Only managers can remove food items
    if (currentRole != Role::MANAGER || currentUserId == -1) {
        return false;
    }
    
    return dbManager->deleteFoodItem(id);
}

// CEO functionality
bool CRMSystem::addManager(const std::string& name, double salary, int startHour, int endHour) {
    // Only CEOs can add managers
    if (currentRole != Role::CEO || currentUserId == -1) {
        return false;
    }
    
    int id = getNextId("managers");
    Manager manager(id, name, salary, startHour, endHour);
    return dbManager->addManager(manager);
}

bool CRMSystem::updateManagerShift(int managerId, int startHour, int endHour) {
    // Only CEOs can update manager shifts
    if (currentRole != Role::CEO || currentUserId == -1) {
        return false;
    }
    
    return dbManager->updateManagerShift(managerId, startHour, endHour);
}

bool CRMSystem::updateManagerSalary(int managerId, double salary) {
    // Only CEOs can update manager salaries
    if (currentRole != Role::CEO || currentUserId == -1) {
        return false;
    }
    
    return dbManager->updateManagerSalary(managerId, salary);
}

// Common functionality
std::vector<std::shared_ptr<FoodItem>> CRMSystem::getMenu() const {
    return dbManager->getAllFoodItems();
}

bool CRMSystem::placeOrder(int tableId, const std::vector<std::pair<int, int>>& foodItems) {
    // Check if table exists
    auto table = dbManager->getTable(tableId);
    if (!table) {
        return false;
    }
    
    // Create order
    std::time_t now = std::time(nullptr);
    std::tm* localTime = std::localtime(&now);
    
    std::stringstream timeStream;
    timeStream << std::put_time(localTime, "%Y-%m-%d %H:%M:%S");
    std::string timeStr = timeStream.str();
    
    int orderId = getNextId("orders");
    Order order(orderId, tableId, timeStr);
    
    // Add items to order
    double totalAmount = 0.0;
    for (const auto& item : foodItems) {
        int foodId = item.first;
        int quantity = item.second;
        
        auto foodItem = dbManager->getFoodItem(foodId);
        if (!foodItem) {
            return false;
        }
        
        order.addItem(foodId, quantity);
        totalAmount += foodItem->getPrice() * quantity;
    }
    
    order.setTotalAmount(totalAmount);
    
    return dbManager->addOrder(order);
}

// System setup
bool CRMSystem::setupTables() {
    // First, disable foreign key constraints temporarily
    sqlite3_exec(dbManager->db, "PRAGMA foreign_keys = OFF;", nullptr, nullptr, nullptr);
    
    auto tables = dbManager->getAllTables();
    
    // If we already have 64 tables, we don't need to create any more
    if (tables.size() >= 64) {
        // Re-enable foreign key constraints
        sqlite3_exec(dbManager->db, "PRAGMA foreign_keys = ON;", nullptr, nullptr, nullptr);
        return true;
    }
    
    // Create tables one by one
    bool success = true;
    for (int i = 1; i <= 64; i++) {
        // Check if this table already exists
        bool exists = false;
        for (const auto& table : tables) {
            if (table->getId() == i) {
                exists = true;
                break;
            }
        }
        
        if (!exists) {
            Table table(i);
            success = dbManager->addTable(table) && success;
            
            // If we encounter an error, stop and report it
            if (!success) {
                std::cerr << "Failed to create table " << i << std::endl;
                break;
            }
        }
    }
    
    // Re-enable foreign key constraints
    sqlite3_exec(dbManager->db, "PRAGMA foreign_keys = ON;", nullptr, nullptr, nullptr);
    
    return success;
}

// Helper methods
int CRMSystem::getNextId(const std::string& tableName) const {
    return dbManager->getNextId(tableName);
}