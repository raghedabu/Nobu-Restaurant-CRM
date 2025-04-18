# Nobu Restaurant CRM System

A simple Customer Relationship Management (CRM) system for Nobu Restaurant, built with C++ and SQLite.

## System Overview

This CRM system manages the following aspects of the restaurant:

- Employee management (waiters, managers, CEO)
- Customer management
- Food menu management
- Order processing
- Table management (64 tables)
- Reservation handling

## Key Features

- Role-based access control: Different access levels for waiters, managers, and CEO
- Table assignments: Waiters are assigned to specific tables
- Shift management: Track employee shifts (simple start/end hours)
- Reservation system: Allow customers to reserve tables if available
- Order management: Track customer orders and food items

## Role-specific Functionality

### Customer
- View available tables
- Make reservations
- View menu
- Place orders

### Waiter
- View orders for their assigned tables

### Manager
- Add/manage waiters (shift, salary)
- Assign tables to waiters
- Manage food items (add, update, remove)

### CEO
- Add/manage managers (shift, salary)

## Project Structure

```
nobu_crm/
│
├── include/
│   ├── models/              # Class definitions
│   │   ├── person.h
│   │   ├── employee.h
│   │   ├── waiter.h
│   │   ├── manager.h
│   │   ├── ceo.h
│   │   ├── customer.h
│   │   ├── food_item.h
│   │   ├── order.h
│   │   ├── table.h
│   │   └── reservation.h
│   │
│   ├── database/            # Database related headers
│   │   └── db_manager.h
│   │
│   └── system/              # System related headers
│       └── crm_system.h
│
├── src/
│   ├── models/              # Class implementations
│   │   ├── person.cpp
│   │   ├── employee.cpp
│   │   ├── waiter.cpp
│   │   ├── manager.cpp
│   │   ├── ceo.cpp
│   │   ├── customer.cpp
│   │   ├── food_item.cpp
│   │   ├── order.cpp
│   │   ├── table.cpp
│   │   └── reservation.cpp
│   │
│   ├── database/            # Database implementations
│   │   └── db_manager.cpp
│   │
│   └── system/              # System implementations
│       └── crm_system.cpp
│
├── sql/                     # SQL files for database schema
│   └── schema.sql           # Main schema file
│
├── main.cpp                 # Main application entry point
├── CMakeLists.txt           # CMake build configuration
└── README.md                # Project documentation
```

## Setup and Build Instructions

### Prerequisites

- C++ compiler with C++14 support
- CMake (version 3.10 or higher)
- SQLite3 development libraries

### Building on Linux/macOS

```bash
# Create a build directory
mkdir build && cd build

# Generate build files
cmake ..

# Build the project
cmake --build .

# Run the application
./NobuCRM
```

### Building on Windows with Visual Studio Code

1. Install Visual Studio Code with C++ and CMake extensions
2. Open the project folder in VS Code
3. Configure CMake (Ctrl+Shift+P -> CMake: Configure)
4. Build the project (Ctrl+Shift+P -> CMake: Build)
5. Run the application (Ctrl+Shift+P -> CMake: Run Without Debugging)

## Future Enhancements

This CRM system has been designed to be compatible with a future HTML/CSS/JavaScript GUI. The current implementation is a simple command-line interface, but the backend structure supports expansion to a web-based interface.

## Notes

- The system is intentionally kept simple as per requirements
- No payment methods, taxes, or tips are included
- Basic data validation is implemented but could be enhanced
- The database is a local SQLite file (nobu_crm.db)
- Shifts are represented simply as start and end hours (0-23)