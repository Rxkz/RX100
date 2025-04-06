#ifndef ORDER_H
#define ORDER_H
#include "File_Z.h"
#include <string>
#include <vector>
#include <sstream>
using namespace std;
using namespace TwoCli;
class Order : public stbase {
public:
    int id;
    int tableId;
    vector<string> items; // Format: "menuItemId:quantity"
    string status; // "Pending" or "Completed"
    double amount;
    string timestamp;
    string specialInstructions; // New field for special instructions

    Order() : id(0), tableId(0), status("Pending"), amount(0.0), specialInstructions("") {}

    string toString() const override {
        string result = to_string(id) + "|" + to_string(tableId) + "|";
        // Serialize the items vector
        for (size_t i = 0; i < items.size(); ++i) {
            result += items[i];
            if (i < items.size() - 1) {
                result += ",";
            }
        }
        result += "|" + status + "|" + to_string(amount) + "|" + timestamp + "|" + specialInstructions;
        return result;
    }

    static Order fromString(const string& line) {
        Order order;
        stringstream ss(line);
        string part;
        getline(ss, part, '|');
        order.id = stoi(part);
        getline(ss, part, '|');
        order.tableId = stoi(part);
        getline(ss, part, '|');
        stringstream itemsSS(part);
        string item;
        while (getline(itemsSS, item, ',')) {
            if (!item.empty()) {
                order.items.push_back(item);
            }
        }
        getline(ss, part, '|');
        order.status = part;
        getline(ss, part, '|');
        order.amount = stod(part);
        getline(ss, part, '|');
        order.timestamp = part;

        // Read special instructions if available
        if (getline(ss, part, '|')) {
            order.specialInstructions = part;
        }
        return order;
    }
};
#endif // ORDER_H