#include "File_Z.h"
#include "Table.h"
#include "MenuItem.h"
#include "Order.h"
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <map>
#include <functional>

using namespace std;
using namespace TwoCli;

// Function decz
void displayTables(const vector<Table>& tables);
void displayMenu(const vector<MenuItem>& menuItems);
double calculateDailyIncome(const vector<Order>& orders);
int findMostOrderedItem(const vector<Order>& orders, const vector<MenuItem>& menuItems);

// Utility functions for the Manager Interface
void clearScreenAndWait() {
    cout << "\nPress Enter to continue...";
    cin.get();
    system("cls");
}

void viewAllTables(FileManager<Table>& tableManager) {
    system("cls");
    cout << "===== ALL TABLES =====" << endl;
    vector<Table> tables = tableManager.readRecords();
    displayTables(tables);
    clearScreenAndWait();
}

void addNewTable(FileManager<Table>& tableManager) {
    system("cls");
    cout << "===== ADD NEW TABLE =====" << endl;
    Table newTable;
    newTable.id = tableManager.getNextId();

    cout << "Enter number of seats: ";
    cin >> newTable.seats;
    cin.ignore();

    newTable.status = "Available";
    newTable.guests = 0;

    tableManager.addRecord(newTable);
    cout << "Table added successfully!" << endl;
    clearScreenAndWait();
}

void editTable(FileManager<Table>& tableManager) {
    system("cls");
    cout << "===== EDIT TABLE =====" << endl;
    vector<Table> tables = tableManager.readRecords();
    displayTables(tables);

    int tableId;
    cout << "\nEnter Table ID to edit: ";
    cin >> tableId;
    cin.ignore();

    bool tableFound = false;
    for (auto& table : tables) {
        if (table.id == tableId) {
            cout << "Enter new number of seats: ";
            cin >> table.seats;
            cin.ignore();

            tableManager.writeRecords(tables);
            cout << "Table updated successfully!" << endl;
            tableFound = true;
            break;
        }
    }

    if (!tableFound) {
        cout << "Table not found." << endl;
    }

    clearScreenAndWait();
}

void deleteTable(FileManager<Table>& tableManager) {
    system("cls");
    cout << "===== DELETE TABLE =====" << endl;
    vector<Table> tables = tableManager.readRecords();
    displayTables(tables);

    int tableId;
    cout << "\nEnter Table ID to delete: ";
    cin >> tableId;
    cin.ignore();

    bool tableFound = false;
    for (auto it = tables.begin(); it != tables.end(); ++it) {
        if (it->id == tableId) {
            tables.erase(it);
            tableManager.writeRecords(tables);
            cout << "Table deleted successfully!" << endl;
            tableFound = true;
            break;
        }
    }

    if (!tableFound) {
        cout << "Table not found." << endl;
    }

    clearScreenAndWait();
}

void viewMenuItems(FileManager<MenuItem>& menuManager) {
    system("cls");
    cout << "===== MENU ITEMS =====" << endl;
    vector<MenuItem> menuItems = menuManager.readRecords();
    displayMenu(menuItems);
    clearScreenAndWait();
}

void addMenuItem(FileManager<MenuItem>& menuManager) {
    system("cls");
    cout << "===== ADD MENU ITEM =====" << endl;
    MenuItem newItem;
    newItem.id = menuManager.getNextId();

    cout << "Enter item name: ";
    getline(cin, newItem.name);

    cout << "Enter item category: ";
    getline(cin, newItem.category);

    cout << "Enter item price: $";
    cin >> newItem.price;
    cin.ignore();

    menuManager.addRecord(newItem);
    cout << "Menu item added successfully!" << endl;
    clearScreenAndWait();
}

void editMenuItem(FileManager<MenuItem>& menuManager) {
    system("cls");
    cout << "===== EDIT MENU ITEM =====" << endl;
    vector<MenuItem> menuItems = menuManager.readRecords();
    displayMenu(menuItems);

    int itemId;
    cout << "\nEnter Menu Item ID to edit: ";
    cin >> itemId;
    cin.ignore();

    bool itemFound = false;
    for (auto& item : menuItems) {
        if (item.id == itemId) {
            cout << "Enter new name (current: " << item.name << "): ";
            getline(cin, item.name);

            cout << "Enter new category (current: " << item.category << "): ";
            getline(cin, item.category);

            cout << "Enter new price (current: $" << fixed << setprecision(2) << item.price << "): $";
            cin >> item.price;
            cin.ignore();

            menuManager.writeRecords(menuItems);
            cout << "Menu item updated successfully!" << endl;
            itemFound = true;
            break;
        }
    }

    if (!itemFound) {
        cout << "Menu item not found." << endl;
    }

    clearScreenAndWait();
}

void deleteMenuItem(FileManager<MenuItem>& menuManager) {
    system("cls");
    cout << "===== DELETE MENU ITEM =====" << endl;
    vector<MenuItem> menuItems = menuManager.readRecords();
    displayMenu(menuItems);

    int itemId;
    cout << "\nEnter Menu Item ID to delete: ";
    cin >> itemId;
    cin.ignore();

    bool itemFound = false;
    for (size_t i = 0; i < menuItems.size(); i++) {
        if (menuItems[i].id == itemId) {
            menuItems.erase(menuItems.begin() + i);
            menuManager.writeRecords(menuItems);
            cout << "Menu item deleted successfully!" << endl;
            itemFound = true;
            break;
        }
    }

    if (!itemFound) {
        cout << "Menu item not found." << endl;
    }

    clearScreenAndWait();
}

void viewTableOccupancy(FileManager<Table>& tableManager) {
    system("cls");
    cout << "===== TABLE OCCUPANCY REPORT =====" << endl;
    vector<Table> tables = tableManager.readRecords();

    int totalTables = tables.size();
    int occupiedTables = 0;
    int totalSeats = 0;
    int occupiedSeats = 0;

    for (const auto& table : tables) {
        totalSeats += table.seats;
        if (table.status == "Occupied") {
            occupiedTables++;
            occupiedSeats += table.guests;
        }
    }

    cout << "Total Tables: " << totalTables << endl;
    cout << "Occupied Tables: " << occupiedTables << " ("
        << fixed << setprecision(1) << (totalTables > 0 ? (occupiedTables * 100.0 / totalTables) : 0)
        << "%)" << endl;
    cout << "Available Tables: " << (totalTables - occupiedTables) << endl;
    cout << "Total Seats: " << totalSeats << endl;
    cout << "Occupied Seats: " << occupiedSeats << " ("
        << fixed << setprecision(1) << (totalSeats > 0 ? (occupiedSeats * 100.0 / totalSeats) : 0)
        << "%)" << endl;

    clearScreenAndWait();
}

void viewMostOrderedItem(FileManager<Order>& orderManager, FileManager<MenuItem>& menuManager) {
    system("cls");
    cout << "===== MOST ORDERED ITEM REPORT =====" << endl;
    vector<Order> orders = orderManager.readRecords();
    vector<MenuItem> menuItems = menuManager.readRecords();

    int mostOrderedId = findMostOrderedItem(orders, menuItems);

    if (mostOrderedId > 0) {
        MenuItem mostOrderedItem;
        for (const auto& item : menuItems) {
            if (item.id == mostOrderedId) {
                mostOrderedItem = item;
                break;
            }
        }

        cout << "Most Ordered Item: " << mostOrderedItem.name << endl;
        cout << "Category: " << mostOrderedItem.category << endl;
        cout << "Price: $" << fixed << setprecision(2) << mostOrderedItem.price << endl;
    }
    else {
        cout << "No orders found to determine the most ordered item." << endl;
    }

    clearScreenAndWait();
}

void viewDailyIncome(FileManager<Order>& orderManager) {
    system("cls");
    cout << "===== DAILY INCOME REPORT =====" << endl;
    vector<Order> orders = orderManager.readRecords();

    double totalIncome = calculateDailyIncome(orders);

    cout << "Today's Income: $" << fixed << setprecision(2) << totalIncome << endl;
    cout << "Number of Orders: " << orders.size() << endl;

    if (!orders.empty()) {
        cout << "Average Order Value: $" << fixed << setprecision(2)
            << (totalIncome / orders.size()) << endl;
    }

    clearScreenAndWait();
}

// Menu handlers
void handleTableManagement(FileManager<Table>& tableManager, FileManager<MenuItem>& menuManager) {
    system("cls");

    // Using a map to store menu and in it is a lambda function {option [capture](parameters) { body }}
    // it is easy as there is a direct connection with the menu option and the function it will be easy also to change it later on by modify the lambda function
    map<int, function<void()>> menuOptions = {
        {1, [&tableManager]() { viewAllTables(tableManager); }},
        {2, [&tableManager]() { addNewTable(tableManager); }},
        {3, [&tableManager]() { editTable(tableManager); }},
       {4, [&tableManager]() { deleteTable(tableManager); }}, // deleteTable waas menu item before thats why i was not able to show table data and del 
        {5, []() { system("cls"); }}
    };

    bool validChoice = false;
    while (!validChoice) {
        cout << "===== TABLE MANAGEMENT =====" << endl;
        cout << "1. View All Tables" << endl;
        cout << "2. Add New Table" << endl;
        cout << "3. Edit Table" << endl;
        cout << "4. Delete Table" << endl;
        cout << "5. Return to Manager Menu" << endl;
        cout << "Enter your choice: ";

        int tableChoice;
        cin >> tableChoice;
        cin.ignore();

        auto it = menuOptions.find(tableChoice);
        if (it != menuOptions.end()) {
            it->second();
            validChoice = true;
        }
        else {
            cout << "Invalid choice. Please try again." << endl;
            clearScreenAndWait();
        }
    }
}

void handleMenuManagement(FileManager<MenuItem>& menuManager) {
    system("cls");


    // Using a map to store menu and in it is a lambda function {option [capture](parameters) { body }}
    // it is easy as there is a direct connection with the menu option and the function it will be easy also to change it later on by modify the lambda function
    map<int, function<void()>> menuOptions = {
        {1, [&menuManager]() { viewMenuItems(menuManager); }},
        {2, [&menuManager]() { addMenuItem(menuManager); }},
        {3, [&menuManager]() { editMenuItem(menuManager); }},
        {4, [&menuManager]() { deleteMenuItem(menuManager); }},
        {5, []() { system("cls"); }}
    };

    bool validChoice = false;
    while (!validChoice) {
        cout << "===== MENU MANAGEMENT =====" << endl;
        cout << "1. View Menu Items" << endl;
        cout << "2. Add Menu Item" << endl;
        cout << "3. Edit Menu Item" << endl;
        cout << "4. Delete Menu Item" << endl;
        cout << "5. Return to Manager Menu" << endl;
        cout << "Enter your choice: ";

        int menuChoice;
        cin >> menuChoice;
        cin.ignore();

        auto it = menuOptions.find(menuChoice);
        if (it != menuOptions.end()) {
            it->second();
            validChoice = true;
        }
        else {
            cout << "Invalid choice. Please try again." << endl;
            clearScreenAndWait();
        }
    }
}

void handleReports(FileManager<Table>& tableManager, FileManager<MenuItem>& menuManager, FileManager<Order>& orderManager) {
    system("cls");
    

    // Using a map to store menu and in it is a lambda function {option [capture](parameters) { body }}
    // it is easy as there is a direct connection with the menu option and the function it will be easy also to change it later on by modify the lambda function

    map<int, function<void()>> menuOptions = {
        {1, [&tableManager]() { viewTableOccupancy(tableManager); }},
        {2, [&orderManager, &menuManager]() { viewMostOrderedItem(orderManager, menuManager); }},
        {3, [&orderManager]() { viewDailyIncome(orderManager); }},
        {4, []() { system("cls"); }}
    };

    bool validChoice = false;
    while (!validChoice) {
        cout << "===== REPORTS =====" << endl;
        cout << "1. Table Occupancy" << endl;
        cout << "2. Most Ordered Item" << endl;
        cout << "3. Daily Income" << endl;
        cout << "4. Return to Manager Menu" << endl;
        cout << "Enter your choice: ";

        int reportChoice;
        cin >> reportChoice;
        cin.ignore();

        auto it = menuOptions.find(reportChoice);
        if (it != menuOptions.end()) {
            it->second();
            validChoice = true;
        }
        else {
            cout << "Invalid choice. Please try again." << endl;
            clearScreenAndWait();
        }
    }
}

// Manager Interface - Main Function
void managerInterface() {
    system("cls");

    FileManager<Table> tableManager("tables.txt");
    FileManager<MenuItem> menuManager("menu.txt");
    FileManager<Order> orderManager("orders.txt");


    // Using a map to store menu and in it is a lambda function {option [capture](parameters) { body }}
    // it is easy as there is a direct connection with the menu option and the function it will be easy also to change it later on by modify the lambda function

    map<int, function<void()>> mainMenuOptions = {
        {1, [&tableManager, &menuManager]() { handleTableManagement(tableManager, menuManager); }},
        {2, [&menuManager]() { handleMenuManagement(menuManager); }},
        {3, [&tableManager, &menuManager, &orderManager]() { handleReports(tableManager, menuManager, orderManager); }},
        {4, []() { system("cls"); }}
    };

    bool exit = false;
    while (!exit) {
        cout << "===== MANAGER TERMINAL =====" << endl;
        cout << "1. Manage Tables" << endl;
        cout << "2. Manage Menu" << endl;
        cout << "3. View Reports" << endl;
        cout << "4. Return to Main Menu" << endl;
        cout << "Enter your choice: ";

        int choice;
        cin >> choice;
        cin.ignore();

        auto it = mainMenuOptions.find(choice);
        if (it != mainMenuOptions.end()) {
            if (choice == 4) {
                exit = true;
            }
            else {
                it->second();
            }
        }
        else {
            cout << "Invalid choice. Please try again." << endl;
            clearScreenAndWait();
        }
    }
}