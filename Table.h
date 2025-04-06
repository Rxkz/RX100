#ifndef TABLE_H
#define TABLE_H

#include "File_Z.h"
#include <string>
#include <sstream>

using namespace std;
using namespace TwoCli;

class Table : public stbase {
public:
    int id;
    int seats;
    string status; // "Available" or "Occupied"
    int guests;

    Table() : id(0), seats(0), status("Available"), guests(0) {}

    string toString() const override {
        return to_string(id) + "|" + to_string(seats) + "|" + status + "|" + to_string(guests);
    }

    static Table fromString(const string& line) {
        Table table;
        stringstream ss(line);
        string part;

        getline(ss, part, '|');
        table.id = stoi(part);

        getline(ss, part, '|');
        table.seats = stoi(part);

        getline(ss, part, '|');
        table.status = part;

        getline(ss, part, '|');
        table.guests = stoi(part);

        return table;
    }
};

#endif // TABLE_H