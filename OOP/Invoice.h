#ifndef INVOICE_H
#define INVOICE_H

#include <vector>
#include <string>
#include "OrderItem.h"
using namespace std;

class Invoice {
private:
    string id;
    string datetime;
    vector<OrderItem> items;
public:
    Invoice() {}
    Invoice(const vector<OrderItem>& _items);

    double total() const;
    string toString() const;
    void print() const;
    bool saveToFile(const string& filename) const;
};

#endif
