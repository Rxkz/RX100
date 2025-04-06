# Restaurant Management System

A comprehensive C++ console application for managing restaurant operations including table management, order processing, and reporting.

## Project Overview

The Restaurant Management System (RMS) is designed to streamline operations for a restaurant by providing separate interfaces for waiters, kitchen staff, and managers. The system manages tables, menu items, and orders through a simple console-based interface.

## Features

### Waiter Terminal
- View available tables
- Assign tables to guests
- Place orders for occupied tables
- Add special instructions to orders

### Kitchen Terminal
- View pending orders in real-time
- Update order status to "Completed"
- Monitor new incoming orders

### Manager Terminal
- Manage restaurant tables (add, edit, delete)
- Manage menu items (add, edit, delete)
- View reports:
  - Table occupancy
  - Most ordered items
  - Daily income

## System Architecture

The system follows object-oriented design principles:
- **Abstraction**: Uses abstract base class (`stbase`) for data models
- **Encapsulation**: Each class encapsulates its data and behavior
- **Inheritance**: Model classes inherit from the base class
- **Polymorphism**: Utilizes template classes to handle different record types

### Class Structure

- **stbase**: Abstract base class with string conversion interface
- **FileManager**: Template class for persistent storage
- **Table**: Represents restaurant tables
- **MenuItem**: Represents menu items
- **Order**: Represents customer orders

## Getting Started

### Prerequisites
- C++ compiler (supports C++11 or later)
- Windows OS (for system("cls") functionality)

### Compilation
```bash
g++ -o restaurant RX100.cpp kitchen.cpp manager.cpp utility.cpp waiter.cpp -std=c++11
```

### Running the Application
```bash
./restaurant
```

## File Structure

- **RX100.cpp**: Entry point and main menu
- **waiter.cpp**: Waiter interface implementation
- **kitchen.cpp**: Kitchen interface implementation
- **manager.cpp**: Manager interface implementation
- **utility.cpp**: Shared utility functions
- **File_Z.h**: File management system
- **Table.h**: Table data model
- **MenuItem.h**: Menu item data model
- **Order.h**: Order data model

## Data Storage

All data is stored in plain text files:
- **tables.txt**: Restaurant tables data
- **menu.txt**: Menu items data
- **orders.txt**: Customer orders data

## Code Examples

### Creating a New Order
```cpp
Order newOrder;
newOrder.id = orderManager.getNextId();
newOrder.tableId = tableId;
newOrder.status = "Pending";
newOrder.timestamp = stbase::getCurrentTimeMark();

// Add items to order
string orderItemStr = to_string(menuItemId) + ":" + to_string(quantity);
newOrder.items.push_back(orderItemStr);
```

### Reading Records from File
```cpp
FileManager<MenuItem> menuManager("menu.txt");
vector<MenuItem> menuItems = menuManager.readRecords();
```

## Contributing

Feel free to submit issues or pull requests for enhancements or bug fixes.

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Acknowledgments

- Developed as part of a programming course project
- Inspired by real-world restaurant management needs
