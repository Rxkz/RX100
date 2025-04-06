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
void waiterInterface();
void kitchenInterface();
void managerInterface();
void displayMainMenu();


int main() {
    int choice;
    bool exit = false;

    while (!exit) {
        displayMainMenu();
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(); // Clear input buffer

        switch (choice) {
        case 1:
            waiterInterface();
            break;
        case 2:
            kitchenInterface();
            break;
        case 3:
            managerInterface();
            break;
        case 4:
            exit = true;
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
            break;
        }
    }

    return 0;
}

// Main menu display
void displayMainMenu() {
    system("cls");

    cout << "===== EATS & TREATS RESTAURANT MANAGEMENT SYSTEM =====" << endl;
    cout << "1. Waiter Terminal" << endl;
    cout << "2. Kitchen Terminal" << endl;
    cout << "3. Manager Terminal" << endl;
    cout << "4. Exit" << endl;
}