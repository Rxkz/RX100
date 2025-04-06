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

using namespace std;
using namespace TwoCli;

// Function prototypes
MenuItem findMenuItem(int id, const vector<MenuItem>& menuItems);

// Display all tables
void displayTables(const vector<Table>& tables) {
    if (tables.empty()) {
        cout << "No tables found." << endl;
        return;
    }

    cout << setw(5) << "ID" << setw(10) << "Seats" << setw(15) << "Status" << setw(10) << "Guests" << endl;
    cout << string(40, '-') << endl;

    for (const auto& table : tables) {
        cout << setw(5) << table.id
            << setw(10) << table.seats
            << setw(15) << table.status
            << setw(10) << table.guests << endl;
    }
}

// Display all menu items
void displayMenu(const vector<MenuItem>& menuItems) {
    if (menuItems.empty()) {
        cout << "No menu items found." << endl;
        return;
    }

    cout << setw(5) << "ID" << setw(25) << "Name" << setw(15) << "Category" << setw(10) << "Price" << endl;
    cout << string(55, '-') << endl;

    for (const auto& item : menuItems) {
        cout << setw(5) << item.id
            << setw(25) << item.name
            << setw(15) << item.category
            << setw(10) << "$" << fixed << setprecision(2) << item.price << endl;
    }
}

// Display all orders
void displayOrders(const vector<Order>& orders) {
    if (orders.empty()) {
        cout << "No orders found." << endl;
        return;
    }

    for (const auto& order : orders) {
        cout << "Order #" << order.id << " - Table: " << order.tableId << endl;
        cout << "Status: " << order.status << endl;
        cout << "Time: " << order.timestamp << endl;
        cout << "Items:" << endl;

        FileManager<MenuItem> menuManager("menu.txt");
        vector<MenuItem> menuItems = menuManager.readRecords();

        for (const auto& itemStr : order.items) {
            size_t colonPos = itemStr.find(':');
            if (colonPos != string::npos) {
                int menuItemId = static_cast<int>(stol(itemStr.substr(0, colonPos)));
                int quantity = static_cast<int>(stol(itemStr.substr(colonPos + 1)));

                // Validate to prevent overflow or negative values
                if (menuItemId < 0 || quantity < 0) {
                    cout << "Error: Invalid menu item ID or quantity in order." << endl;
                    continue;
                }

                MenuItem item = findMenuItem(menuItemId, menuItems);
                cout << "  " << quantity << "x " << item.name << " ($" << fixed << setprecision(2) << item.price << " each)" << endl;
            }
        }

        cout << "Total Amount: $" << fixed << setprecision(2) << order.amount << endl;
        // Add this after displaying the total amount in displayOrders()
        if (!order.specialInstructions.empty()) {
            cout << "Special Instructions: " << order.specialInstructions << endl;
        }

        cout << "------------------------" << endl;
    }
}

// Find a menu item by ID
MenuItem findMenuItem(int id, const vector<MenuItem>& menuItems) {
    for (const auto& item : menuItems) {
        if (item.id == id) {
            return item;
        }
    }

    MenuItem emptyItem;
    emptyItem.id = 0;
    emptyItem.name = "Unknown Item";
    emptyItem.price = 0.0;
    return emptyItem;
}

// Calculate total income for the day
double calculateDailyIncome(const vector<Order>& orders) {
    double totalIncome = 0.0;

    for (const auto& order : orders) {
        if (order.status == "Completed") {
            totalIncome += order.amount;
        }
    }

    return totalIncome;
}

// Find the most ordered menu item
int findMostOrderedItem(const vector<Order>& orders, const vector<MenuItem>& menuItems) {
    if (orders.empty() || menuItems.empty()) {
        return 0;
    }

    // Count occurrences of each menu item
    map<int, int> itemCounts;

    for (const auto& order : orders) {
        for (const auto& itemStr : order.items) {
            size_t colonPos = itemStr.find(':');
            if (colonPos != string::npos) {
                int menuItemId = stoi(itemStr.substr(0, colonPos));
                int quantity = stoi(itemStr.substr(colonPos + 1));

                itemCounts[menuItemId] += quantity;
            }
        }
    }

    // Find the item with the highest count
    int mostOrderedId = 0;
    int highestCount = 0;

    for (const auto& pair : itemCounts) {
        if (pair.second > highestCount) {
            highestCount = pair.second;
            mostOrderedId = pair.first;
        }
    }

    return mostOrderedId;
}