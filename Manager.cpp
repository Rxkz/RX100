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
double calculateDailyIncome(const vector<Order>& orders);
int findMostOrderedItem(const vector<Order>& orders, const vector<MenuItem>& menuItems);



// Manager Interface
void managerInterface() {
    system("cls");
    
    FileManager<Table> tableManager("tables.txt");
    FileManager<MenuItem> menuManager("menu.txt");
    FileManager<Order> orderManager("orders.txt");

    int choice;
    bool exit = false;

    while (!exit) {
        cout << "===== MANAGER TERMINAL =====" << endl;
        cout << "1. Manage Tables" << endl;
        cout << "2. Manage Menu" << endl;
        cout << "3. View Reports" << endl;
        cout << "4. Return to Main Menu" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(); // Clear input buffer

        switch (choice) {
        case 1: {
            int tableChoice;
            system("cls");
            cout << "===== TABLE MANAGEMENT =====" << endl;
            cout << "1. View All Tables" << endl;
            cout << "2. Add New Table" << endl;
            cout << "3. Edit Table" << endl;
            cout << "4. Delete Table" << endl;
            cout << "5. Return to Manager Menu" << endl;
            cout << "Enter your choice: ";
            cin >> tableChoice;
            cin.ignore();

            switch (tableChoice) {
            case 1: {
                system("cls");
                cout << "===== ALL TABLES =====" << endl;
                vector<Table> tables = tableManager.readRecords();
                displayTables(tables);
                cout << "\nPress Enter to continue...";
                cin.get();
                system("cls");
                break;
            }
            case 2: {
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

                cout << "\nPress Enter to continue...";
                cin.get();
                system("cls");
                break;
            }
            case 3: {
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

                cout << "\nPress Enter to continue...";
                cin.get();
                system("cls");
                break;
            }
            case 4: {
                system("cls");
                cout << "===== DELETE MENU ITEM =====" << endl;
                vector<MenuItem> menuItems = menuManager.readRecords();
                displayMenu(menuItems);

                int itemId;
                cout << "\nEnter Menu Item ID to delete: ";
                cin >> itemId;
                cin.ignore();

                bool itemFound = false;
                for (auto it = menuItems.begin(); it != menuItems.end(); ++it) {
                    if (it->id == itemId) {
                        menuItems.erase(it);
                        menuManager.writeRecords(menuItems);
                        cout << "Menu item deleted successfully!" << endl;
                        itemFound = true;
                        break;
                    }
                }

                if (!itemFound) {
                    cout << "Menu item not found." << endl;
                }

                cout << "\nPress Enter to continue...";
                cin.get();
                system("cls");
                break;
            }
            case 5:
                system("cls");
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
                cout << "\nPress Enter to continue...";
                cin.get();
                system("cls");
                break;
            }
            break;
        }
        case 2: {
            int menuChoice;
            system("cls");
            cout << "===== MENU MANAGEMENT =====" << endl;
            cout << "1. View Menu Items" << endl;
            cout << "2. Add Menu Item" << endl;
            cout << "3. Edit Menu Item" << endl;
            cout << "4. Delete Menu Item" << endl;
            cout << "5. Return to Manager Menu" << endl;
            cout << "Enter your choice: ";
            cin >> menuChoice;
            cin.ignore();

            switch (menuChoice) {
            case 1: {
                system("cls");
                cout << "===== MENU ITEMS =====" << endl;
                vector<MenuItem> menuItems = menuManager.readRecords();
                displayMenu(menuItems);
                cout << "\nPress Enter to continue...";
                cin.get();
                system("cls");
                break;
            }
            case 2: {
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

                cout << "\nPress Enter to continue...";
                cin.get();
                system("cls");
                break;
            }
            case 3: {
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

                cout << "\nPress Enter to continue...";
                cin.get();
                system("cls");
                break;
            }
            case 4: {
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

                cout << "\nPress Enter to continue...";
                cin.get();
                system("cls");
                break;
            }
            case 5:
                system("cls");
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
                cout << "\nPress Enter to continue...";
                cin.get();
                system("cls");
                break;
            }
            break;
        }
        case 3: {
            int reportChoice;
            system("cls");
            cout << "===== REPORTS =====" << endl;
            cout << "1. Table Occupancy" << endl;
            cout << "2. Most Ordered Item" << endl;
            cout << "3. Daily Income" << endl;
            cout << "4. Return to Manager Menu" << endl;
            cout << "Enter your choice: ";
            cin >> reportChoice;
            cin.ignore();

            switch (reportChoice) {
            case 1: {
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

                cout << "\nPress Enter to continue...";
                cin.get();
                system("cls");
                break;
            }
            case 2: {
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

                cout << "\nPress Enter to continue...";
                cin.get();
                system("cls");
                break;
            }
            case 3: {
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

                cout << "\nPress Enter to continue...";
                cin.get();
                system("cls");
                break;
            }
            case 4:
                system("cls");
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
                cout << "\nPress Enter to continue...";
                cin.get();
                system("cls");
                break;
            }
            break;
        }
        case 4:
            exit = true;
            system("cls");
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
            cout << "\nPress Enter to continue...";
            cin.get();
            system("cls");
            break;
        }
    }
}