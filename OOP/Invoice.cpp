#include <bits/stdc++.h>
#include "Material.h"
#include "Inventory.h"
#include "OrderItem.h"
#include "Invoice.h"
#include "InputHelper.h"
#include "Flows.h"
using namespace std;

Invoice::Invoice(const vector<OrderItem>& _items) : items(_items) {
    time_t now = time(nullptr);
    id = "INV" + to_string(now);
    char buf[64];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
    datetime = buf;
}

double Invoice::total() const {
    double s = 0;
    for (const auto& it : items) s += it.subtotal();
    return s;
}

string Invoice::toString() const {
    ostringstream oss;
    oss << "Hoa don ID: " << id << "\nNgay: " << datetime << "\n";
    oss << left << setw(10) << "ID" << left << setw(25) << "Ten" << left << setw(8) << "Don vi"
        << right << setw(10) << "Gia" << right << setw(8) << "SL" << right << setw(12) << "Thanh tien" << "\n";
    oss << string(75, '-') << "\n";
    for (const auto& it : items) {
        oss << left << setw(10) << it.id
            << left << setw(25) << it.name
            << left << setw(8) << it.unit
            << right << setw(10) << fixed << setprecision(2) << it.unitPrice
            << right << setw(8) << it.qty
            << right << setw(12) << fixed << setprecision(2) << it.subtotal() << "\n";
    }
    oss << string(75, '-') << "\n";
    oss << right << setw(60) << "TONG: " << setw(12) << fixed << setprecision(2) << total() << "\n";
    return oss.str();
}

void Invoice::print() const {
    cout << toString();
}

bool Invoice::saveToFile(const string& filename) const {
    ofstream ofs(filename, ios::app);
    if (!ofs) return false;
    ofs << toString() << "\n===\n";
    return true;
}
