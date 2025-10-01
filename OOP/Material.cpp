#include <bits/stdc++.h>
#include "Material.h"
#include "Inventory.h"
#include "OrderItem.h"
#include "Invoice.h"
#include "InputHelper.h"
#include "Flows.h"
using namespace std;

Material::Material() : price(0.0), stock(0) {}
Material::Material(const string& _id, const string& _name, const string& _unit, double _price, int _stock)
    : id(_id), name(_name), unit(_unit), price(_price), stock(_stock) {}

string Material::getId() const { return id; }
string Material::getName() const { return name; }
string Material::getUnit() const { return unit; }
double Material::getPrice() const { return price; }
int Material::getStock() const { return stock; }

void Material::setName(const string& s) { name = s; }
void Material::setUnit(const string& s) { unit = s; }
void Material::setPrice(double p) { price = p; }
void Material::setStock(int s) { stock = s; }

string Material::serialize() const {
    ostringstream oss;
    oss << id << "|" << name << "|" << unit << "|"
        << fixed << setprecision(2) << price << "|" << stock;
    return oss.str();
}

bool Material::deserialize(const string& line, Material& out) {
    if (line.empty()) return false;
    stringstream ss(line);
    string id_, name_, unit_, priceStr, stockStr;
    if (!getline(ss, id_, '|')) return false;
    if (!getline(ss, name_, '|')) return false;
    if (!getline(ss, unit_, '|')) return false;
    if (!getline(ss, priceStr, '|')) return false;
    if (!getline(ss, stockStr)) return false;
    try {
        double p = stod(priceStr);
        int s = stoi(stockStr);
        out = Material(id_, name_, unit_, p, s);
        return true;
    } catch (...) {
        return false;
    }
}

void Material::printTableRow() const {
    cout << left << setw(10) << id
         << left << setw(25) << name
         << left << setw(8) << unit
         << right << setw(10) << fixed << setprecision(2) << price
         << right << setw(8) << stock << "\n";
}
