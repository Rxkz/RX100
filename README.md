# Restaurant Management System

A comprehensive C++ console application for managing restaurant operations including table management, order processing, and reporting.

## Project Overview

The Restaurant Management System (RMS) is designed to streamline operations for a restaurant by providing separate interfaces for waiters, kitchen staff, and managers. The system manages tables, menu items, and orders through a simple console-based interface.

## UML Class Diagram
![Editor _ Mermaid Chart-2025-04-06-045524](https://github.com/user-attachments/assets/58b6f778-c9be-4b9c-92a2-ae71c075594d)



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

### OOP Concepts Used

#### 1. Abstraction
- The `stbase` class is an abstract class that defines a common interface with the pure virtual method `toString()`.
- Abstraction is used to hide the complex implementation details of record serialization while providing a simple interface.
- Each model class implements its own version of `toString()` and provides a static `fromString()` method.

#### 2. Encapsulation
- All classes encapsulate their data by providing public methods to access and manipulate the data.
- `FileManager` class encapsulates file operations, hiding the complexity of file I/O, thread synchronization, and error handling.
- The model classes (Table, MenuItem, Order) encapsulate their state and provide methods to convert to/from string representation.

#### 3. Inheritance
- The model classes (`Table`, `MenuItem`, and `Order`) inherit from the abstract base class `stbase`.
- This inheritance relationship allows for common functionality like serialization to be defined once in the base class.
- Each derived class overrides the `toString()` method to provide its specific implementation.

#### 4. Polymorphism
- **Runtime Polymorphism**: The `toString()` method is overridden in each derived class, allowing different implementations to be called through the same interface.
- **Compile-time Polymorphism**: The `FileManager` template class uses parametric polymorphism to work with different record types.
- Interface functions use the same methods (like `displayTables()`) across different contexts.

#### 5. Composition/Aggregation
- `Order` class has a composition relationship with `MenuItem` objects, as it contains a collection of menu item references.
- The management system uses aggregation to create relationships between orders and tables.

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

## Design Principles

### Interface Segregation
- The system is divided into separate interfaces (Waiter, Kitchen, Manager) that provide specific functionality to different user roles.
- Each interface exposes only the methods relevant to its specific role.

### Single Responsibility Principle
- Each class has a single responsibility:
  - `FileManager` handles file operations
  - Model classes (`Table`, `MenuItem`, `Order`) represent data structures
  - Interface classes handle user interactions for specific roles

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
