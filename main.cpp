// main.cpp
#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <limits>
#include "include/system/crm_system.h"

// Helper functions for demonstration
void displayMenu() {
    std::cout << "\n======== Nobu Restaurant CRM System ========\n";
    std::cout << "1. Register as Customer\n";
    std::cout << "2. Login as Customer\n";
    std::cout << "3. Login as Waiter\n";
    std::cout << "4. Login as Manager\n";
    std::cout << "5. Login as CEO\n";
    std::cout << "6. Exit\n";
    std::cout << "Enter your choice: ";
}

// Function to clear input buffer
void clearInputBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void handleCustomerMenu(CRMSystem& system) {
    int choice;
    
    while (true) {
        std::cout << "\n======== Customer Menu ========\n";
        std::cout << "1. View Available Tables\n";
        std::cout << "2. Make Reservation\n";
        std::cout << "3. View Menu\n";
        std::cout << "4. Place Order\n";
        std::cout << "5. Logout\n";
        std::cout << "Enter your choice: ";
        
        // Handle invalid input
        if (!(std::cin >> choice)) {
            std::cout << "Invalid input. Please enter a number.\n";
            clearInputBuffer();
            continue;
        }
        
        if (choice == 1) {
            auto tables = system.getAvailableTables();
            std::cout << "\nAvailable Tables:\n";
            for (const auto& table : tables) {
                std::cout << "Table " << table->getId() << std::endl;
            }
        }
        else if (choice == 2) {
            int tableId;
            std::string time;
            
            std::cout << "Enter table ID: ";
            std::cin >> tableId;
            
            std::cout << "Enter time (YYYY-MM-DD HH:MM:SS): ";
            clearInputBuffer();
            std::getline(std::cin, time);
            
            if (system.makeReservation(tableId, time)) {
                std::cout << "Reservation successful!\n";
            } else {
                std::cout << "Failed to make reservation. Table might be unavailable.\n";
            }
        }
        else if (choice == 3) {
            auto menu = system.getMenu();
            std::cout << "\nMenu:\n";
            for (const auto& item : menu) {
                std::cout << item->getId() << ". " << item->getName() 
                          << " - $" << item->getPrice() << std::endl;
            }
        }
        else if (choice == 4) {
            int tableId;
            int numItems;
            
            std::cout << "Enter table ID: ";
            std::cin >> tableId;
            
            std::cout << "How many different items would you like to order? ";
            std::cin >> numItems;
            
            std::vector<std::pair<int, int>> orderItems;
            for (int i = 0; i < numItems; i++) {
                int foodId, quantity;
                
                std::cout << "Enter food item ID: ";
                std::cin >> foodId;
                
                std::cout << "Enter quantity: ";
                std::cin >> quantity;
                
                orderItems.push_back({foodId, quantity});
            }
            
            if (system.placeOrder(tableId, orderItems)) {
                std::cout << "Order placed successfully!\n";
            } else {
                std::cout << "Failed to place order.\n";
            }
        }
        else if (choice == 5) {
            system.logout();
            break;
        }
        else {
            std::cout << "Invalid choice. Please try again.\n";
        }
    }
}

void handleWaiterMenu(CRMSystem& system) {
    int choice;
    
    while (true) {
        std::cout << "\n======== Waiter Menu ========\n";
        std::cout << "1. View Orders for Table\n";
        std::cout << "2. Logout\n";
        std::cout << "Enter your choice: ";
        
        // Handle invalid input
        if (!(std::cin >> choice)) {
            std::cout << "Invalid input. Please enter a number.\n";
            clearInputBuffer();
            continue;
        }
        
        if (choice == 1) {
            int tableId;
            std::cout << "Enter table ID: ";
            std::cin >> tableId;
            
            std::vector<std::shared_ptr<Order>> orders;
            if (system.viewOrdersForTable(tableId, orders)) {
                std::cout << "\nOrders for Table " << tableId << ":\n";
                for (const auto& order : orders) {
                    std::cout << "Order " << order->getId() << " - Time: " << order->getTime() 
                              << " - Total: $" << order->getTotalAmount() 
                              << " - Paid: " << (order->getIsPaid() ? "Yes" : "No") << std::endl;
                    
                    std::cout << "Items:\n";
                    for (const auto& item : order->getItems()) {
                        std::cout << "Food ID: " << item.foodId << " - Quantity: " << item.quantity << std::endl;
                    }
                    std::cout << "-----------------\n";
                }
            } else {
                std::cout << "Failed to view orders. Table might not be assigned to you.\n";
            }
        }
        else if (choice == 2) {
            system.logout();
            break;
        }
        else {
            std::cout << "Invalid choice. Please try again.\n";
        }
    }
}

void handleManagerMenu(CRMSystem& system) {
    int choice;
    
    while (true) {
        std::cout << "\n======== Manager Menu ========\n";
        std::cout << "1. Add Waiter\n";
        std::cout << "2. Update Waiter Shift\n";
        std::cout << "3. Update Waiter Salary\n";
        std::cout << "4. Assign Table to Waiter\n";
        std::cout << "5. Add Food Item\n";
        std::cout << "6. Update Food Item\n";
        std::cout << "7. Remove Food Item\n";
        std::cout << "8. Logout\n";
        std::cout << "Enter your choice: ";
        
        // Handle invalid input
        if (!(std::cin >> choice)) {
            std::cout << "Invalid input. Please enter a number.\n";
            clearInputBuffer();
            continue;
        }
        
        if (choice == 1) {
            std::string name;
            double salary;
            int startHour, endHour;
            
            std::cout << "Enter waiter name: ";
            clearInputBuffer();
            std::getline(std::cin, name);
            
            std::cout << "Enter salary: ";
            std::cin >> salary;
            
            std::cout << "Enter start hour (0-23): ";
            std::cin >> startHour;
            
            std::cout << "Enter end hour (0-23): ";
            std::cin >> endHour;
            
            if (system.addWaiter(name, salary, startHour, endHour)) {
                std::cout << "Waiter added successfully!\n";
            } else {
                std::cout << "Failed to add waiter.\n";
            }
        }
        else if (choice == 2) {
            int waiterId, startHour, endHour;
            
            std::cout << "Enter waiter ID: ";
            std::cin >> waiterId;
            
            std::cout << "Enter new start hour (0-23): ";
            std::cin >> startHour;
            
            std::cout << "Enter new end hour (0-23): ";
            std::cin >> endHour;
            
            if (system.updateWaiterShift(waiterId, startHour, endHour)) {
                std::cout << "Waiter shift updated successfully!\n";
            } else {
                std::cout << "Failed to update waiter shift.\n";
            }
        }
        else if (choice == 3) {
            int waiterId;
            double salary;
            
            std::cout << "Enter waiter ID: ";
            std::cin >> waiterId;
            
            std::cout << "Enter new salary: ";
            std::cin >> salary;
            
            if (system.updateWaiterSalary(waiterId, salary)) {
                std::cout << "Waiter salary updated successfully!\n";
            } else {
                std::cout << "Failed to update waiter salary.\n";
            }
        }
        else if (choice == 4) {
            int tableId, waiterId;
            
            std::cout << "Enter table ID: ";
            std::cin >> tableId;
            
            std::cout << "Enter waiter ID: ";
            std::cin >> waiterId;
            
            if (system.assignTableToWaiter(tableId, waiterId)) {
                std::cout << "Table assigned to waiter successfully!\n";
            } else {
                std::cout << "Failed to assign table to waiter.\n";
            }
        }
        else if (choice == 5) {
            std::string name;
            double price;
            
            std::cout << "Enter food item name: ";
            clearInputBuffer();
            std::getline(std::cin, name);
            
            std::cout << "Enter price: ";
            std::cin >> price;
            
            if (system.addFoodItem(name, price)) {
                std::cout << "Food item added successfully!\n";
            } else {
                std::cout << "Failed to add food item.\n";
            }
        }
        else if (choice == 6) {
            int foodId;
            std::string name;
            double price;
            
            std::cout << "Enter food item ID: ";
            std::cin >> foodId;
            
            std::cout << "Enter new name: ";
            clearInputBuffer();
            std::getline(std::cin, name);
            
            std::cout << "Enter new price: ";
            std::cin >> price;
            
            if (system.updateFoodItem(foodId, name, price)) {
                std::cout << "Food item updated successfully!\n";
            } else {
                std::cout << "Failed to update food item.\n";
            }
        }
        else if (choice == 7) {
            int foodId;
            
            std::cout << "Enter food item ID: ";
            std::cin >> foodId;
            
            if (system.removeFoodItem(foodId)) {
                std::cout << "Food item removed successfully!\n";
            } else {
                std::cout << "Failed to remove food item.\n";
            }
        }
        else if (choice == 8) {
            system.logout();
            break;
        }
        else {
            std::cout << "Invalid choice. Please try again.\n";
        }
    }
}

void handleCEOMenu(CRMSystem& system) {
    int choice;
    
    while (true) {
        std::cout << "\n======== CEO Menu ========\n";
        std::cout << "1. Add Manager\n";
        std::cout << "2. Update Manager Shift\n";
        std::cout << "3. Update Manager Salary\n";
        std::cout << "4. Logout\n";
        std::cout << "Enter your choice: ";
        
        // Handle invalid input
        if (!(std::cin >> choice)) {
            std::cout << "Invalid input. Please enter a number.\n";
            clearInputBuffer();
            continue;
        }
        
        if (choice == 1) {
            std::string name;
            double salary;
            int startHour, endHour;
            
            std::cout << "Enter manager name: ";
            clearInputBuffer();
            std::getline(std::cin, name);
            
            std::cout << "Enter salary: ";
            std::cin >> salary;
            
            std::cout << "Enter start hour (0-23): ";
            std::cin >> startHour;
            
            std::cout << "Enter end hour (0-23): ";
            std::cin >> endHour;
            
            if (system.addManager(name, salary, startHour, endHour)) {
                std::cout << "Manager added successfully!\n";
            } else {
                std::cout << "Failed to add manager.\n";
            }
        }
        else if (choice == 2) {
            int managerId, startHour, endHour;
            
            std::cout << "Enter manager ID: ";
            std::cin >> managerId;
            
            std::cout << "Enter new start hour (0-23): ";
            std::cin >> startHour;
            
            std::cout << "Enter new end hour (0-23): ";
            std::cin >> endHour;
            
            if (system.updateManagerShift(managerId, startHour, endHour)) {
                std::cout << "Manager shift updated successfully!\n";
            } else {
                std::cout << "Failed to update manager shift.\n";
            }
        }
        else if (choice == 3) {
            int managerId;
            double salary;
            
            std::cout << "Enter manager ID: ";
            std::cin >> managerId;
            
            std::cout << "Enter new salary: ";
            std::cin >> salary;
            
            if (system.updateManagerSalary(managerId, salary)) {
                std::cout << "Manager salary updated successfully!\n";
            } else {
                std::cout << "Failed to update manager salary.\n";
            }
        }
        else if (choice == 4) {
            system.logout();
            break;
        }
        else {
            std::cout << "Invalid choice. Please try again.\n";
        }
    }
}

int main() {
    // Initialize CRM system
    CRMSystem system("nobu_crm.db");
    if (!system.initializeSystem()) {
        std::cerr << "Failed to initialize CRM system. Exiting...\n";
        return 1;
    }
    
    // Set up initial users for testing if they don't exist
    auto dbManager = system.getDbManager();
    if (!dbManager->getCEO(1)) {
        CEO ceo(1, "Default CEO", 10000.0, 9, 17);
        dbManager->addCEO(ceo);
        std::cout << "Created default CEO with ID 1\n";
    }
    
    if (!dbManager->getManager(1)) {
        Manager manager(1, "Default Manager", 5000.0, 9, 17);
        dbManager->addManager(manager);
        std::cout << "Created default Manager with ID 1\n";
    }
    
    // Add some sample food items if menu is empty
    auto foodItems = dbManager->getAllFoodItems();
    if (foodItems.empty()) {
        dbManager->addFoodItem(FoodItem(1, "Sushi Platter", 45.99));
        dbManager->addFoodItem(FoodItem(2, "Tempura", 18.50));
        dbManager->addFoodItem(FoodItem(3, "Miso Soup", 6.99));
        dbManager->addFoodItem(FoodItem(4, "Edamame", 7.50));
        dbManager->addFoodItem(FoodItem(5, "Black Cod", 36.99));
        std::cout << "Added sample food items to the menu\n";
    }
    
    std::cout << "Welcome to Nobu Restaurant CRM System!\n";
    
    int choice;
    while (true) {
        displayMenu();
        
        // Handle invalid input
        if (!(std::cin >> choice)) {
            std::cout << "Invalid input. Please enter a number.\n";
            clearInputBuffer();
            continue;
        }
        
        if (choice == 1) {
            // Register as customer
            std::string name;
            std::cout << "Enter your name: ";
            clearInputBuffer();
            std::getline(std::cin, name);
            
            if (system.registerCustomer(name)) {
                std::cout << "Registration successful!\n";
            } else {
                std::cout << "Registration failed.\n";
            }
        }
        else if (choice == 2) {
            // Login as customer
            int customerId;
            std::cout << "Enter your customer ID: ";
            std::cin >> customerId;
            
            if (system.login(Role::CUSTOMER, customerId)) {
                std::cout << "Login successful!\n";
                handleCustomerMenu(system);
            } else {
                std::cout << "Login failed. Invalid customer ID.\n";
            }
        }
        else if (choice == 3) {
            // Login as waiter
            int waiterId;
            std::cout << "Enter your waiter ID: ";
            std::cin >> waiterId;
            
            if (system.login(Role::WAITER, waiterId)) {
                std::cout << "Login successful!\n";
                handleWaiterMenu(system);
            } else {
                std::cout << "Login failed. Invalid waiter ID.\n";
            }
        }
        else if (choice == 4) {
            // Login as manager
            int managerId;
            std::cout << "Enter your manager ID: ";
            std::cin >> managerId;
            
            if (system.login(Role::MANAGER, managerId)) {
                std::cout << "Login successful!\n";
                handleManagerMenu(system);
            } else {
                std::cout << "Login failed. Invalid manager ID.\n";
            }
        }
        else if (choice == 5) {
            // Login as CEO
            int ceoId;
            std::cout << "Enter your CEO ID: ";
            std::cin >> ceoId;
            
            if (system.login(Role::CEO, ceoId)) {
                std::cout << "Login successful!\n";
                handleCEOMenu(system);
            } else {
                std::cout << "Login failed. Invalid CEO ID.\n";
            }
        }
        else if (choice == 6) {
            std::cout << "Thank you for using Nobu Restaurant CRM System. Goodbye!\n";
            break;
        }
        else {
            std::cout << "Invalid choice. Please try again.\n";
        }
    }
    
    return 0;
}