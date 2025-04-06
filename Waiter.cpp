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
void displayTables(const vector<Table>& tables);
void displayMenu(const vector<MenuItem>& menuItems);
void displayOrders(const vector<Order>& orders);
MenuItem findMenuItem(int id, const vector<MenuItem>& menuItems);

// Waiter Interface
void waiterInterface() {
    system("cls");
    FileManager<Table> tableManager("tables.txt");
    FileManager<MenuItem> menuManager("menu.txt");
    FileManager<Order> orderManager("orders.txt");

    int choice;
    bool exit = false;

    while (!exit) {
       
       
        cout << "===== WAITER TERMINAL =====" << endl;
        cout << "1. View Tables" << endl;
        cout << "2. Assign Table to Guests" << endl;
        cout << "3. Place Order" << endl;  
        cout << "4. Return to Main Menu" << endl;  
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(); // Clear input buffer

        switch (choice) {
        case 1: {
            system("cls");
            cout << "===== TABLE STATUS =====" << endl;
            vector<Table> tables = tableManager.readRecords();
            displayTables(tables);
            cout << "\nPress Enter to continue...";
            cin.get();
            system("cls");
            break;
        }
        case 2: {
            system("cls");
            cout << "===== ASSIGN TABLE =====" << endl;
            vector<Table> tables = tableManager.readRecords();
            displayTables(tables);

            int tableId;
            cout << "\nEnter table ID to assign: ";
            cin >> tableId;
            cin.ignore();

            // Find the table
            bool found = false;
            for (auto& table : tables) {
                if (table.id == tableId) {
                    if (table.status == "Available") {
                        int guests;
                        cout << "Enter number of guests: ";
                        cin >> guests;
                        cin.ignore();

                        table.status = "Occupied";
                        table.guests = guests;
                        tableManager.writeRecords(tables);
                        cout << "Table " << tableId << " assigned successfully!" << endl;
                    }
                    else {
                        cout << "Table " << tableId << " is not available." << endl;
                    }
                    found = true;
                    break;
                }
            }

            if (!found) {
                cout << "Table ID not found." << endl;
            }

            cout << "\nPress Enter to continue...";
            cin.get();
            system("cls");
            break;
        }
        
        case 3: {
            system("cls");
            cout << "===== PLACE ORDER =====" << endl;
            vector<Table> tables = tableManager.readRecords();
            vector<MenuItem> menuItems = menuManager.readRecords();

            // Display available tables
            cout << "Available Tables:" << endl;
            bool hasAvailableTables = false;
            for (const auto& table : tables) {
                if (table.status == "Occupied") {
                    cout << "Table " << table.id << " (Seats: " << table.seats << ", Guests: " << table.guests << ")" << endl;
                    hasAvailableTables = true;
                }
            }

            if (!hasAvailableTables) {
                cout << "No occupied tables available for orders." << endl;
                cout << "\nPress Enter to continue...";
                cin.get();
                system("cls");
                break;
            }

            int tableId;
            cout << "\nEnter table ID for the order: ";
            cin >> tableId;
            cin.ignore();

            // Verify table exists and is occupied
            bool tableValid = false;
            for (const auto& table : tables) {
                if (table.id == tableId && table.status == "Occupied") {
                    tableValid = true;
                    break;
                }
            }

            if (!tableValid) {
                cout << "Invalid table or table not occupied." << endl;
                cout << "\nPress Enter to continue...";
                cin.get();
                system("cls");
                break;
            }

            // Display menu for selection
            displayMenu(menuItems);

            // Create new order
            Order newOrder;
            newOrder.id = orderManager.getNextId();
            newOrder.tableId = tableId;
            newOrder.status = "Pending";
            newOrder.timestamp = stbase::getCurrentTimeMark();

            // Add items to order
            bool addingItems = true;
            double totalAmount = 0.0;
            while (addingItems) {
                int menuItemId;
                int quantity;

                cout << "\nEnter Menu Item ID (0 to finish): ";
                cin >> menuItemId;
                cin.ignore();

                if (menuItemId == 0) {
                    addingItems = false;
                    continue;
                }

                // Find the menu item
                bool itemFound = false;
                MenuItem selectedItem;
                for (const auto& item : menuItems) {
                    if (item.id == menuItemId) {
                        selectedItem = item;
                        itemFound = true;
                        break;
                    }
                }

                if (!itemFound) {
                    cout << "Menu item not found." << endl;
                    continue;
                }

                cout << "Enter quantity: ";
                cin >> quantity;
                cin.ignore();

                // Add item to order
                string orderItemStr = to_string(menuItemId) + ":" + to_string(quantity);
                newOrder.items.push_back(orderItemStr);
                totalAmount += selectedItem.price * quantity;

                cout << "Added " << quantity << "x " << selectedItem.name << " ($" << fixed << setprecision(2) << selectedItem.price << " each)" << endl;
            }

            newOrder.amount = totalAmount;

            cout << "\nAny special instructions? (Press Enter to skip): ";
            string specialInstructions;
            getline(cin, specialInstructions);
            newOrder.specialInstructions = specialInstructions;

            orderManager.addRecord(newOrder);

            cout << "\nOrder placed successfully! Total: $" << fixed << setprecision(2) << totalAmount << endl;
            cout << "\nPress Enter to continue...";
            cin.get();
            system("cls");
            break;
        }
       
        case 4:
            exit = true;
            system("cls");
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
            break;
        }
    }
}