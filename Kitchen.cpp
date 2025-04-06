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
void displayMenu(const vector<MenuItem>& menuItems);

// Kitchen Interface
void kitchenInterface() {
    system("cls");
    FileManager<Order> orderManager("orders.txt");
    FileManager<MenuItem> menuManager("menu.txt");

    cout << "===== KITCHEN TERMINAL =====" << endl;
    cout << "1. View Pending Orders (Live Monitor)" << endl;
    cout << "2. Update Order Status" << endl;
    cout << "3. Return to Main Menu" << endl;

    int choice;
    cout << "Enter your choice: ";
    cin >> choice;
    cin.ignore(); // Clear input buffer

    switch (choice) {
    case 1: {
        // Setup live monitoring of orders
        auto displayPendingOrders = [&menuManager](const vector<Order>& orders) {
            vector<MenuItem> menuItems = menuManager.readRecords();
            cout << "===== PENDING ORDERS =====" << endl;

            bool hasPending = false;
            for (const auto& order : orders) {
                if (order.status == "Pending") {
                    hasPending = true;
                    cout << "Order #" << order.id << " - Table: " << order.tableId
                        << " - Time: " << order.timestamp.substr(11, 8) << endl;

                    cout << "Items:" << endl;
                    for (const auto& itemStr : order.items) {
                        size_t colonPos = itemStr.find(':');
                        if (colonPos != string::npos) {
                            int menuItemId = stoi(itemStr.substr(0, colonPos));
                            int quantity = stoi(itemStr.substr(colonPos + 1));

                            MenuItem item;
                            for (const auto& menuItem : menuItems) {
                                if (menuItem.id == menuItemId) {
                                    item = menuItem;
                                    break;
                                }
                            }

                            cout << "  " << quantity << "x " << item.name << endl;
                        }
                    }

                    if (!order.specialInstructions.empty()) {
                        cout << "Special Instructions: " << order.specialInstructions << endl;
                    }
                    cout << "------------------------" << endl;
                }
            }

            if (!hasPending) {
                cout << "No pending orders." << endl;
            }
            };

        auto hasOrderChanges = [](const vector<Order>& current, const vector<Order>& previous) {
            // Check if the number of orders has changed
            if (current.size() != previous.size()) {
                return true;
            }

            // Check if any order status has changed
            for (size_t i = 0; i < current.size(); i++) {
                if (i >= previous.size() || current[i].status != previous[i].status) {
                    return true;
                }
            }

            return false;
            };

        orderManager.startMonitor(displayPendingOrders, hasOrderChanges, 5);
        system("cls");
        kitchenInterface();
        return;
    }
    case 2: {
        system("cls");
        cout << "===== UPDATE ORDER STATUS =====" << endl;
        vector<Order> orders = orderManager.readRecords();
        vector<MenuItem> menuItems = menuManager.readRecords();

        // Display pending orders
        bool hasPending = false;
        for (const auto& order : orders) {
            if (order.status == "Pending") {
                hasPending = true;
                cout << "Order #" << order.id << " - Table: " << order.tableId << endl;

                // Display ordered items
                for (const auto& itemStr : order.items) {
                    size_t colonPos = itemStr.find(':');
                    if (colonPos != string::npos) {
                        int menuItemId = stoi(itemStr.substr(0, colonPos));
                        int quantity = stoi(itemStr.substr(colonPos + 1));

                        MenuItem item;
                        for (const auto& menuItem : menuItems) {
                            if (menuItem.id == menuItemId) {
                                item = menuItem;
                                break;
                            }
                        }

                        cout << "  " << quantity << "x " << item.name << endl;


                    }
                }

                if (!order.specialInstructions.empty()) {
                    cout << "Special Instructions: " << order.specialInstructions << endl;
                }

                cout << "------------------------" << endl;
            }
        }

        if (!hasPending) {
            cout << "No pending orders to update." << endl;
            cout << "\nPress Enter to continue...";
            cin.get();
            system("cls");
            break;
        }

        int orderId;
        cout << "\nEnter Order ID to mark as Completed: ";
        cin >> orderId;
        cin.ignore();

        bool orderFound = false;
        for (auto& order : orders) {
            if (order.id == orderId && order.status == "Pending") {
                order.status = "Completed";
                orderManager.writeRecords(orders);
                cout << "Order #" << orderId << " marked as Completed." << endl;
                orderFound = true;
                break;
            }
        }

        if (!orderFound) {
            cout << "Order not found or not in Pending status." << endl;
        }

        cout << "\nPress Enter to continue...";
        cin.get();
        system("cls");
        break;
    }
    case 3:
       
        //exitM = true;
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