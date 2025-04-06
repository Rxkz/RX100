#ifndef MENUITEM_H
#define MENUITEM_H

#include "File_Z.h"
#include <string>
#include <sstream>

using namespace std;
using namespace TwoCli;

class MenuItem : public stbase {
public:
    int id;
    string name;
    string category;
    double price;

    MenuItem() : id(0), name(""), category(""), price(0.0) {}

    string toString() const override {
        return to_string(id) + "|" + name + "|" + category + "|" + to_string(price);
    }

    static MenuItem fromString(const string& line) {
        MenuItem item;
        stringstream ss(line);
        string part;

        getline(ss, part, '|');
        item.id = stoi(part);

        getline(ss, part, '|');
        item.name = part;

        getline(ss, part, '|');
        item.category = part;

        getline(ss, part, '|');
        item.price = stod(part);

        return item;
    }
};

#endif // MENUITEM_H