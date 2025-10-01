#ifndef ORDERITEM_H
#define ORDERITEM_H

#include <string>
using namespace std;

struct OrderItem {
    string id;
    string name;
    string unit;
    double unitPrice;
    int qty;

    double subtotal() const { return unitPrice * qty; }
};

#endif
