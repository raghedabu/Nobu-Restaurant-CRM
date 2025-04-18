-- Nobu Restaurant CRM Database Schema

-- Customers table
CREATE TABLE IF NOT EXISTS customers (
    id INTEGER PRIMARY KEY,
    name TEXT NOT NULL
);

-- Waiters table
CREATE TABLE IF NOT EXISTS waiters (
    id INTEGER PRIMARY KEY,
    name TEXT NOT NULL,
    salary REAL NOT NULL,
    start_hour INTEGER NOT NULL,
    end_hour INTEGER NOT NULL
);

-- Waiter-Table relationship
CREATE TABLE IF NOT EXISTS waiter_tables (
    waiter_id INTEGER,
    table_id INTEGER,
    PRIMARY KEY (waiter_id, table_id),
    FOREIGN KEY (waiter_id) REFERENCES waiters (id),
    FOREIGN KEY (table_id) REFERENCES tables (id)
);

-- Managers table
CREATE TABLE IF NOT EXISTS managers (
    id INTEGER PRIMARY KEY,
    name TEXT NOT NULL,
    salary REAL NOT NULL,
    start_hour INTEGER NOT NULL,
    end_hour INTEGER NOT NULL
);

-- CEO table
CREATE TABLE IF NOT EXISTS ceo (
    id INTEGER PRIMARY KEY,
    name TEXT NOT NULL,
    salary REAL NOT NULL,
    start_hour INTEGER NOT NULL,
    end_hour INTEGER NOT NULL
);

-- Food items table
CREATE TABLE IF NOT EXISTS food_items (
    id INTEGER PRIMARY KEY,
    name TEXT NOT NULL,
    price REAL NOT NULL
);

-- Tables table
CREATE TABLE IF NOT EXISTS tables (
    id INTEGER PRIMARY KEY,
    is_reserved INTEGER NOT NULL,
    waiter_id INTEGER,
    FOREIGN KEY (waiter_id) REFERENCES waiters (id)
);

-- Reservations table
CREATE TABLE IF NOT EXISTS reservations (
    id INTEGER PRIMARY KEY,
    customer_id INTEGER NOT NULL,
    table_id INTEGER NOT NULL,
    time TEXT NOT NULL,
    FOREIGN KEY (customer_id) REFERENCES customers (id),
    FOREIGN KEY (table_id) REFERENCES tables (id)
);

-- Orders table
CREATE TABLE IF NOT EXISTS orders (
    id INTEGER PRIMARY KEY,
    table_id INTEGER NOT NULL,
    time TEXT NOT NULL,
    total_amount REAL NOT NULL,
    is_paid INTEGER NOT NULL,
    FOREIGN KEY (table_id) REFERENCES tables (id)
);

-- Order items table
CREATE TABLE IF NOT EXISTS order_items (
    order_id INTEGER NOT NULL,
    food_id INTEGER NOT NULL,
    quantity INTEGER NOT NULL,
    PRIMARY KEY (order_id, food_id),
    FOREIGN KEY (order_id) REFERENCES orders (id),
    FOREIGN KEY (food_id) REFERENCES food_items (id)
);