#include <bits/stdc++.h>
#include "Material.h"
#include "Inventory.h"
#include "OrderItem.h"
#include "Invoice.h"
#include "InputHelper.h"
#include "Flows.h"
using namespace std;

void ignoreLine() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

string inputLine(const string& prompt) {
    cout << prompt;
    string s;
    getline(cin, s);
    return s;
}

int inputInt(const string& prompt) {
    while (true) {
        string s = inputLine(prompt);
        try {
            int v = stoi(s);
            return v;
        } catch (...) {
            cout << "Gia tri khong hop le. Nhap lai.\n";
        }
    }
}

double inputDouble(const string& prompt) {
    while (true) {
        string s = inputLine(prompt);
        try {
            double v = stod(s);
            return v;
        } catch (...) {
            cout << "Gia tri khong hop le. Nhap lai.\n";
        }
    }
}
