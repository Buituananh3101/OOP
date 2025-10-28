#ifndef ORDERITEM_H
#define ORDERITEM_H

#include <string>
using namespace std;

class OrderItem {
    public:
    string id;
    string name;
    string unit;
    double unitPrice;
    int qty;

    double subtotal() const { return unitPrice * qty; }
};

#endif
