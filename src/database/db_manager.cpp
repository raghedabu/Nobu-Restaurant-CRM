// db_manager.cpp
#include "../../include/database/db_manager.h"
#include <iostream>

DBManager::DBManager(const std::string& dbPath) : dbPath(dbPath), db(nullptr) {}

std::vector<std::shared_ptr<Customer>> DBManager::getAllCustomers() {
    std::vector<std::shared_ptr<Customer>> customers;
    std::string sql = "SELECT id, name FROM customers;";
    
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        return customers;
    }
    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const char* name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        
        customers.push_back(std::make_shared<Customer>(id, name));
    }
    
    sqlite3_finalize(stmt);
    return customers;
}

// Waiter methods
bool DBManager::addWaiter(const Waiter& waiter) {
    // Insert the waiter
    std::string sql = "INSERT INTO waiters (id, name, salary, start_hour, end_hour) VALUES (" + 
                      std::to_string(waiter.getId()) + ", '" + 
                      waiter.getName() + "', " +
                      std::to_string(waiter.getSalary()) + ", " +
                      std::to_string(waiter.getStartHour()) + ", " +
                      std::to_string(waiter.getEndHour()) + ");";
    
    if (!executeSQL(sql)) {
        return false;
    }
    
    // Add assigned tables for the waiter
    for (int tableId : waiter.getAssignedTables()) {
        std::string tableSql = "INSERT INTO waiter_tables (waiter_id, table_id) VALUES (" +
                              std::to_string(waiter.getId()) + ", " +
                              std::to_string(tableId) + ");";
        if (!executeSQL(tableSql)) {
            return false;
        }
    }
    
    return true;
}


std::shared_ptr<Waiter> DBManager::getWaiter(int id) {
    std::string sql = "SELECT id, name, salary, start_hour, end_hour FROM waiters WHERE id = " + std::to_string(id) + ";";
    
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        return nullptr;
    }

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        int waiterId = sqlite3_column_int(stmt, 0);
        const char* name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        double salary = sqlite3_column_double(stmt, 2);
        int startHour = sqlite3_column_int(stmt, 3);
        int endHour = sqlite3_column_int(stmt, 4);

        sqlite3_finalize(stmt);

        auto waiter = std::make_shared<Waiter>(waiterId, name, salary, startHour, endHour);

        // Get assigned tables
        std::string tablesSql = "SELECT table_id FROM waiter_tables WHERE waiter_id = " + std::to_string(id) + ";";
        if (sqlite3_prepare_v2(db, tablesSql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                int tableId = sqlite3_column_int(stmt, 0);
                waiter->assignTable(tableId);
            }
            sqlite3_finalize(stmt);
        }

        return waiter;  // Return the waiter object
    }

    sqlite3_finalize(stmt);
    return nullptr;  // Return nullptr if no waiter was found
}


bool DBManager::updateOrderPayment(int id, bool isPaid) {
    std::string sql = "UPDATE orders SET is_paid = " + std::to_string(isPaid ? 1 : 0) + 
                      " WHERE id = " + std::to_string(id) + ";";
    return executeSQL(sql);
}

// Helper method to get next ID
int DBManager::getNextId(const std::string& tableName) const {
    sqlite3_stmt* stmt;
    std::string sql = "SELECT MAX(id) FROM " + tableName + ";";
    
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        return 1; // Start with ID 1 if there's an error
    }
    
    int maxId = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        maxId = sqlite3_column_int(stmt, 0);
    }
    
    sqlite3_finalize(stmt);
    return maxId + 1;
}

std::vector<std::shared_ptr<Waiter>> DBManager::getAllWaiters() {
    std::vector<std::shared_ptr<Waiter>> waiters;
    std::string sql = "SELECT id, name, salary, start_hour, end_hour FROM waiters;";
    
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        return waiters;
    }
    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const char* name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        double salary = sqlite3_column_double(stmt, 2);
        int startHour = sqlite3_column_int(stmt, 3);
        int endHour = sqlite3_column_int(stmt, 4);
        
        waiters.push_back(std::make_shared<Waiter>(id, name, salary, startHour, endHour));
    }
    
    sqlite3_finalize(stmt);
    
    // Load assigned tables for each waiter
    for (auto& waiter : waiters) {
        std::string tablesSql = "SELECT table_id FROM waiter_tables WHERE waiter_id = " + std::to_string(waiter->getId()) + ";";
        if (sqlite3_prepare_v2(db, tablesSql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                int tableId = sqlite3_column_int(stmt, 0);
                waiter->assignTable(tableId);
            }
            
            sqlite3_finalize(stmt);
        }
    }
    
    return waiters;
}

bool DBManager::updateWaiterShift(int id, int startHour, int endHour) {
    std::string sql = "UPDATE waiters SET start_hour = " + std::to_string(startHour) + 
                      ", end_hour = " + std::to_string(endHour) + 
                      " WHERE id = " + std::to_string(id) + ";";
    return executeSQL(sql);
}

bool DBManager::updateWaiterSalary(int id, double salary) {
    std::string sql = "UPDATE waiters SET salary = " + std::to_string(salary) + 
                      " WHERE id = " + std::to_string(id) + ";";
    return executeSQL(sql);
}

bool DBManager::updateWaiterTable(int waiterId, int tableId) {
    // First, remove any existing assignments for this table
    std::string deleteSql = "DELETE FROM waiter_tables WHERE table_id = " + std::to_string(tableId) + ";";
    if (!executeSQL(deleteSql)) {
        return false;
    }
    
    // Then, assign the table to the new waiter
    std::string insertSql = "INSERT INTO waiter_tables (waiter_id, table_id) VALUES (" +
                          std::to_string(waiterId) + ", " +
                          std::to_string(tableId) + ");";
    return executeSQL(insertSql);
}

// Manager methods
bool DBManager::addManager(const Manager& manager) {
    std::string sql = "INSERT INTO managers (id, name, salary, start_hour, end_hour) VALUES (" + 
                      std::to_string(manager.getId()) + ", '" + 
                      manager.getName() + "', " +
                      std::to_string(manager.getSalary()) + ", " +
                      std::to_string(manager.getStartHour()) + ", " +
                      std::to_string(manager.getEndHour()) + ");";
    return executeSQL(sql);
}

std::shared_ptr<Manager> DBManager::getManager(int id) {
    std::string sql = "SELECT id, name, salary, start_hour, end_hour FROM managers WHERE id = " + std::to_string(id) + ";";
    
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        return nullptr;
    }
    
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        int managerId = sqlite3_column_int(stmt, 0);
        const char* name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        double salary = sqlite3_column_double(stmt, 2);
        int startHour = sqlite3_column_int(stmt, 3);
        int endHour = sqlite3_column_int(stmt, 4);
        
        sqlite3_finalize(stmt);
        return std::make_shared<Manager>(managerId, name, salary, startHour, endHour);
    }
    
    sqlite3_finalize(stmt);
    return nullptr;
}

std::vector<std::shared_ptr<Manager>> DBManager::getAllManagers() {
    std::vector<std::shared_ptr<Manager>> managers;
    std::string sql = "SELECT id, name, salary, start_hour, end_hour FROM managers;";
    
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        return managers;
    }
    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const char* name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        double salary = sqlite3_column_double(stmt, 2);
        int startHour = sqlite3_column_int(stmt, 3);
        int endHour = sqlite3_column_int(stmt, 4);
        
        managers.push_back(std::make_shared<Manager>(id, name, salary, startHour, endHour));
    }
    
    sqlite3_finalize(stmt);
    return managers;
}

bool DBManager::updateManagerShift(int id, int startHour, int endHour) {
    std::string sql = "UPDATE managers SET start_hour = " + std::to_string(startHour) + 
                      ", end_hour = " + std::to_string(endHour) + 
                      " WHERE id = " + std::to_string(id) + ";";
    return executeSQL(sql);
}

bool DBManager::updateManagerSalary(int id, double salary) {
    std::string sql = "UPDATE managers SET salary = " + std::to_string(salary) + 
                      " WHERE id = " + std::to_string(id) + ";";
    return executeSQL(sql);
}

// CEO methods
bool DBManager::addCEO(const CEO& ceo) {
    std::string sql = "INSERT INTO ceo (id, name, salary, start_hour, end_hour) VALUES (" + 
                      std::to_string(ceo.getId()) + ", '" + 
                      ceo.getName() + "', " +
                      std::to_string(ceo.getSalary()) + ", " +
                      std::to_string(ceo.getStartHour()) + ", " +
                      std::to_string(ceo.getEndHour()) + ");";
    return executeSQL(sql);
}

std::shared_ptr<CEO> DBManager::getCEO(int id) {
    std::string sql = "SELECT id, name, salary, start_hour, end_hour FROM ceo WHERE id = " + std::to_string(id) + ";";
    
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        return nullptr;
    }
    
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        int ceoId = sqlite3_column_int(stmt, 0);
        const char* name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        double salary = sqlite3_column_double(stmt, 2);
        int startHour = sqlite3_column_int(stmt, 3);
        int endHour = sqlite3_column_int(stmt, 4);
        
        sqlite3_finalize(stmt);
        return std::make_shared<CEO>(ceoId, name, salary, startHour, endHour);
    }
    
    sqlite3_finalize(stmt);
    return nullptr;
}

// Food item methods
bool DBManager::addFoodItem(const FoodItem& foodItem) {
    std::string sql = "INSERT INTO food_items (id, name, price) VALUES (" + 
                      std::to_string(foodItem.getId()) + ", '" + 
                      foodItem.getName() + "', " +
                      std::to_string(foodItem.getPrice()) + ");";
    return executeSQL(sql);
}

bool DBManager::updateFoodItem(const FoodItem& foodItem) {
    std::string sql = "UPDATE food_items SET name = '" + foodItem.getName() + 
                      "', price = " + std::to_string(foodItem.getPrice()) + 
                      " WHERE id = " + std::to_string(foodItem.getId()) + ";";
    return executeSQL(sql);
}

bool DBManager::deleteFoodItem(int id) {
    std::string sql = "DELETE FROM food_items WHERE id = " + std::to_string(id) + ";";
    return executeSQL(sql);
}

std::shared_ptr<FoodItem> DBManager::getFoodItem(int id) {
    std::string sql = "SELECT id, name, price FROM food_items WHERE id = " + std::to_string(id) + ";";
    
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        return nullptr;
    }
    
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        int foodId = sqlite3_column_int(stmt, 0);
        const char* name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        double price = sqlite3_column_double(stmt, 2);
        
        sqlite3_finalize(stmt);
        return std::make_shared<FoodItem>(foodId, name, price);
    }
    
    sqlite3_finalize(stmt);
    return nullptr;
}

std::vector<std::shared_ptr<FoodItem>> DBManager::getAllFoodItems() {
    std::vector<std::shared_ptr<FoodItem>> foodItems;
    std::string sql = "SELECT id, name, price FROM food_items;";
    
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        return foodItems;
    }
    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const char* name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        double price = sqlite3_column_double(stmt, 2);
        
        foodItems.push_back(std::make_shared<FoodItem>(id, name, price));
    }
    
    sqlite3_finalize(stmt);
    return foodItems;
}

// Table methods
bool DBManager::addTable(const Table& table) {
    std::string sql = "INSERT INTO tables (id, is_reserved, waiter_id) VALUES (" + 
                      std::to_string(table.getId()) + ", " + 
                      std::to_string(table.getIsReserved() ? 1 : 0) + ", " +
                      std::to_string(table.getWaiterId()) + ");";
    return executeSQL(sql);
}

std::shared_ptr<Table> DBManager::getTable(int id) {
    std::string sql = "SELECT id, is_reserved, waiter_id FROM tables WHERE id = " + std::to_string(id) + ";";
    
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        return nullptr;
    }
    
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        int tableId = sqlite3_column_int(stmt, 0);
        bool isReserved = sqlite3_column_int(stmt, 1) != 0;
        int waiterId = sqlite3_column_int(stmt, 2);
        
        sqlite3_finalize(stmt);
        
        auto table = std::make_shared<Table>(tableId);
        table->setIsReserved(isReserved);
        table->setWaiterId(waiterId);
        
        return table;
    }
    
    sqlite3_finalize(stmt);
    return nullptr;
}

std::vector<std::shared_ptr<Table>> DBManager::getAllTables() {
    std::vector<std::shared_ptr<Table>> tables;
    std::string sql = "SELECT id, is_reserved, waiter_id FROM tables;";
    
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        return tables;
    }
    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        bool isReserved = sqlite3_column_int(stmt, 1) != 0;
        int waiterId = sqlite3_column_int(stmt, 2);
        
        auto table = std::make_shared<Table>(id);
        table->setIsReserved(isReserved);
        table->setWaiterId(waiterId);
        
        tables.push_back(table);
    }
    
    sqlite3_finalize(stmt);
    return tables;
}

std::vector<std::shared_ptr<Table>> DBManager::getAvailableTables() {
    std::vector<std::shared_ptr<Table>> tables;
    std::string sql = "SELECT id, waiter_id FROM tables WHERE is_reserved = 0;";
    
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        return tables;
    }
    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        int waiterId = sqlite3_column_int(stmt, 1);
        
        auto table = std::make_shared<Table>(id);
        table->setIsReserved(false);
        table->setWaiterId(waiterId);
        
        tables.push_back(table);
    }
    
    sqlite3_finalize(stmt);
    return tables;
}

bool DBManager::updateTableStatus(int id, bool isReserved) {
    std::string sql = "UPDATE tables SET is_reserved = " + std::to_string(isReserved ? 1 : 0) + 
                      " WHERE id = " + std::to_string(id) + ";";
    return executeSQL(sql);
}

bool DBManager::assignWaiterToTable(int tableId, int waiterId) {
    std::string sql = "UPDATE tables SET waiter_id = " + std::to_string(waiterId) + 
                      " WHERE id = " + std::to_string(tableId) + ";";
    
    if (!executeSQL(sql)) {
        return false;
    }
    
    // Also update the waiter_tables relationship
    std::string deleteSql = "DELETE FROM waiter_tables WHERE table_id = " + std::to_string(tableId) + ";";
    if (!executeSQL(deleteSql)) {
        return false;
    }
    
    std::string insertSql = "INSERT INTO waiter_tables (waiter_id, table_id) VALUES (" +
                          std::to_string(waiterId) + ", " +
                          std::to_string(tableId) + ");";
    return executeSQL(insertSql);
}

// Reservation methods
bool DBManager::addReservation(const Reservation& reservation) {
    std::string sql = "INSERT INTO reservations (id, customer_id, table_id, time) VALUES (" + 
                      std::to_string(reservation.getId()) + ", " + 
                      std::to_string(reservation.getCustomerId()) + ", " +
                      std::to_string(reservation.getTableId()) + ", '" +
                      reservation.getTime() + "');";
    
    if (!executeSQL(sql)) {
        return false;
    }
    
    // Update table status to reserved
    return updateTableStatus(reservation.getTableId(), true);
}

std::shared_ptr<Reservation> DBManager::getReservation(int id) {
    std::string sql = "SELECT id, customer_id, table_id, time FROM reservations WHERE id = " + std::to_string(id) + ";";
    
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        return nullptr;
    }
    
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        int reservationId = sqlite3_column_int(stmt, 0);
        int customerId = sqlite3_column_int(stmt, 1);
        int tableId = sqlite3_column_int(stmt, 2);
        const char* time = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        
        sqlite3_finalize(stmt);
        return std::make_shared<Reservation>(reservationId, customerId, tableId, time);
    }
    
    sqlite3_finalize(stmt);
    return nullptr;
}

std::vector<std::shared_ptr<Reservation>> DBManager::getReservationsByCustomer(int customerId) {
    std::vector<std::shared_ptr<Reservation>> reservations;
    std::string sql = "SELECT id, customer_id, table_id, time FROM reservations WHERE customer_id = " + std::to_string(customerId) + ";";
    
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        return reservations;
    }
    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        int custId = sqlite3_column_int(stmt, 1);
        int tableId = sqlite3_column_int(stmt, 2);
        const char* time = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        
        reservations.push_back(std::make_shared<Reservation>(id, custId, tableId, time));
    }
    
    sqlite3_finalize(stmt);
    return reservations;
}

std::vector<std::shared_ptr<Reservation>> DBManager::getReservationsByTable(int tableId) {
    std::vector<std::shared_ptr<Reservation>> reservations;
    std::string sql = "SELECT id, customer_id, table_id, time FROM reservations WHERE table_id = " + std::to_string(tableId) + ";";
    
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        return reservations;
    }
    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        int custId = sqlite3_column_int(stmt, 1);
        int tblId = sqlite3_column_int(stmt, 2);
        const char* time = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        
        reservations.push_back(std::make_shared<Reservation>(id, custId, tblId, time));
    }
    
    sqlite3_finalize(stmt);
    return reservations;
}

// Order methods
bool DBManager::addOrder(const Order& order) {
    std::string sql = "INSERT INTO orders (id, table_id, time, total_amount, is_paid) VALUES (" + 
                      std::to_string(order.getId()) + ", " + 
                      std::to_string(order.getTableId()) + ", '" +
                      order.getTime() + "', " +
                      std::to_string(order.getTotalAmount()) + ", " +
                      std::to_string(order.getIsPaid() ? 1 : 0) + ");";
    
    if (!executeSQL(sql)) {
        return false;
    }
    
    // Add order items
    for (const auto& item : order.getItems()) {
        if (!addOrderItem(order.getId(), item.foodId, item.quantity)) {
            return false;
        }
    }
    
    return true;
}

bool DBManager::addOrderItem(int orderId, int foodId, int quantity) {
    std::string sql = "INSERT INTO order_items (order_id, food_id, quantity) VALUES (" + 
                      std::to_string(orderId) + ", " + 
                      std::to_string(foodId) + ", " +
                      std::to_string(quantity) + ");";
    return executeSQL(sql);
}

std::shared_ptr<Order> DBManager::getOrder(int id) {
    std::string sql = "SELECT id, table_id, time, total_amount, is_paid FROM orders WHERE id = " + std::to_string(id) + ";";
    
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        return nullptr;
    }
    
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        int orderId = sqlite3_column_int(stmt, 0);
        int tableId = sqlite3_column_int(stmt, 1);
        const char* time = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        double totalAmount = sqlite3_column_double(stmt, 3);
        bool isPaid = sqlite3_column_int(stmt, 4) != 0;
        
        sqlite3_finalize(stmt);
        
        auto order = std::make_shared<Order>(orderId, tableId, time);
        order->setTotalAmount(totalAmount);
        order->setPaid(isPaid);
        
        // Get order items
        std::string itemsSql = "SELECT food_id, quantity FROM order_items WHERE order_id = " + std::to_string(id) + ";";
        if (sqlite3_prepare_v2(db, itemsSql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                int foodId = sqlite3_column_int(stmt, 0);
                int quantity = sqlite3_column_int(stmt, 1);
                
                order->addItem(foodId, quantity);
            }
            
            sqlite3_finalize(stmt);
        }
        
        return order;
    }
    
    sqlite3_finalize(stmt);
    return nullptr;
}

std::vector<std::shared_ptr<Order>> DBManager::getOrdersByTable(int tableId) {
    std::vector<std::shared_ptr<Order>> orders;
    std::string sql = "SELECT id, table_id, time, total_amount, is_paid FROM orders WHERE table_id = " + std::to_string(tableId) + ";";
    
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        return orders;
    }
    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        int tblId = sqlite3_column_int(stmt, 1);
        const char* time = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        double totalAmount = sqlite3_column_double(stmt, 3);
        bool isPaid = sqlite3_column_int(stmt, 4) != 0;
        
        auto order = std::make_shared<Order>(id, tblId, time);
        order->setTotalAmount(totalAmount);
        order->setPaid(isPaid);
        
        orders.push_back(order);
    }
    
    sqlite3_finalize(stmt);
    
    // Get order items for each order
    for (auto& order : orders) {
        std::string itemsSql = "SELECT food_id, quantity FROM order_items WHERE order_id = " + std::to_string(order->getId()) + ";";
        if (sqlite3_prepare_v2(db, itemsSql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                int foodId = sqlite3_column_int(stmt, 0);
                int quantity = sqlite3_column_int(stmt, 1);
                
                order->addItem(foodId, quantity);
            }
            
            sqlite3_finalize(stmt);
        }
    }
    
    return orders;
}

DBManager::~DBManager() {
    if (db) {
        sqlite3_close(db);
        db = nullptr;
    }
}
        

bool DBManager::executeSQL(const std::string& sql) {
    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
    
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }
    
    return true;
}

bool DBManager::initialize() {
    int rc = sqlite3_open(dbPath.c_str(), &db);
    
    if (rc != SQLITE_OK) {
        std::cerr << "Cannot open database: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return false;
    }
    
    // Enable foreign key constraints
    executeSQL("PRAGMA foreign_keys = OFF;");  // Temporarily disable for setup
    
    // Create tables in the correct order (dependencies first)
    // 1. Create independent tables first
    std::string createCustomersTable = 
        "CREATE TABLE IF NOT EXISTS customers ("
        "id INTEGER PRIMARY KEY,"
        "name TEXT NOT NULL);";
    
    std::string createWaitersTable = 
        "CREATE TABLE IF NOT EXISTS waiters ("
        "id INTEGER PRIMARY KEY,"
        "name TEXT NOT NULL,"
        "salary REAL NOT NULL,"
        "start_hour INTEGER NOT NULL,"
        "end_hour INTEGER NOT NULL);";
    
    std::string createManagersTable = 
        "CREATE TABLE IF NOT EXISTS managers ("
        "id INTEGER PRIMARY KEY,"
        "name TEXT NOT NULL,"
        "salary REAL NOT NULL,"
        "start_hour INTEGER NOT NULL,"
        "end_hour INTEGER NOT NULL);";
    
    std::string createCEOTable = 
        "CREATE TABLE IF NOT EXISTS ceo ("
        "id INTEGER PRIMARY KEY,"
        "name TEXT NOT NULL,"
        "salary REAL NOT NULL,"
        "start_hour INTEGER NOT NULL,"
        "end_hour INTEGER NOT NULL);";
    
    std::string createFoodItemsTable = 
        "CREATE TABLE IF NOT EXISTS food_items ("
        "id INTEGER PRIMARY KEY,"
        "name TEXT NOT NULL,"
        "price REAL NOT NULL);";
    
    // 2. Create tables with dependencies next
    std::string createTablesTable = 
        "CREATE TABLE IF NOT EXISTS tables ("
        "id INTEGER PRIMARY KEY,"
        "is_reserved INTEGER NOT NULL,"
        "waiter_id INTEGER,"
        "FOREIGN KEY (waiter_id) REFERENCES waiters (id));";
    
    std::string createWaiterTablesTable = 
        "CREATE TABLE IF NOT EXISTS waiter_tables ("
        "waiter_id INTEGER,"
        "table_id INTEGER,"
        "PRIMARY KEY (waiter_id, table_id),"
        "FOREIGN KEY (waiter_id) REFERENCES waiters (id),"
        "FOREIGN KEY (table_id) REFERENCES tables (id));";
    
    std::string createReservationsTable = 
        "CREATE TABLE IF NOT EXISTS reservations ("
        "id INTEGER PRIMARY KEY,"
        "customer_id INTEGER NOT NULL,"
        "table_id INTEGER NOT NULL,"
        "time TEXT NOT NULL,"
        "FOREIGN KEY (customer_id) REFERENCES customers (id),"
        "FOREIGN KEY (table_id) REFERENCES tables (id));";
    
    std::string createOrdersTable = 
        "CREATE TABLE IF NOT EXISTS orders ("
        "id INTEGER PRIMARY KEY,"
        "table_id INTEGER NOT NULL,"
        "time TEXT NOT NULL,"
        "total_amount REAL NOT NULL,"
        "is_paid INTEGER NOT NULL,"
        "FOREIGN KEY (table_id) REFERENCES tables (id));";
    
    std::string createOrderItemsTable = 
        "CREATE TABLE IF NOT EXISTS order_items ("
        "order_id INTEGER NOT NULL,"
        "food_id INTEGER NOT NULL,"
        "quantity INTEGER NOT NULL,"
        "PRIMARY KEY (order_id, food_id),"
        "FOREIGN KEY (order_id) REFERENCES orders (id),"
        "FOREIGN KEY (food_id) REFERENCES food_items (id));";
    
    // Execute in the correct order - first independent tables, then dependent ones
    bool success = 
        executeSQL(createCustomersTable) &&
        executeSQL(createWaitersTable) &&
        executeSQL(createManagersTable) &&
        executeSQL(createCEOTable) &&
        executeSQL(createFoodItemsTable) &&
        executeSQL(createTablesTable);
        
    if (!success) return false;
        
    success = success &&
        executeSQL(createWaiterTablesTable) &&
        executeSQL(createReservationsTable) &&
        executeSQL(createOrdersTable) &&
        executeSQL(createOrderItemsTable);
        
    // Re-enable foreign key constraints
    executeSQL("PRAGMA foreign_keys = ON;");
        
    return success;
}

// Customer methods
bool DBManager::addCustomer(const Customer& customer) {
    std::string sql = "INSERT INTO customers (id, name) VALUES (" + 
                      std::to_string(customer.getId()) + ", '" + 
                      customer.getName() + "');";
    return executeSQL(sql);
}

std::shared_ptr<Customer> DBManager::getCustomer(int id) {
    std::string sql = "SELECT id, name FROM customers WHERE id = " + std::to_string(id) + ";";
    
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        return nullptr;
    }
    
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        int customerId = sqlite3_column_int(stmt, 0);
        const char* name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        
        sqlite3_finalize(stmt);
        return std::make_shared<Customer>(customerId, name);
    }
    
    sqlite3_finalize(stmt);
    return nullptr;
}