#include <bits/stdc++.h>
#include "Material.h"
#include "Inventory.h"
#include "OrderItem.h"
#include "Invoice.h"
#include "InputHelper.h"
#include "Flows.h"
using namespace std;

int Inventory::findIndexById(const string& id) const {
    for (size_t i = 0; i < items.size(); ++i)
        if (items[i].getId() == id) return (int)i;
    return -1;
}

Material* Inventory::getMaterialPtr(const string& id) {
    int idx = findIndexById(id);
    if (idx == -1) return nullptr;
    return &items[idx];
}

bool Inventory::addMaterial(const Material& m) {
    if (findIndexById(m.getId()) != -1) return false;
    items.push_back(m);
    return true;
}

bool Inventory::removeMaterial(const string& id) {
    int idx = findIndexById(id);
    if (idx == -1) return false;
    items.erase(items.begin() + idx);
    return true;
}

bool Inventory::updateMaterial(const string& id, const string& name, const string& unit, double price, int stock) {
    Material* p = getMaterialPtr(id);
    if (!p) return false;
    p->setName(name);
    p->setUnit(unit);
    p->setPrice(price);
    p->setStock(stock);
    return true;
}

bool Inventory::restock(const string& id, int qty) {
    Material* p = getMaterialPtr(id);
    if (!p) return false;
    p->setStock(p->getStock() + qty);
    return true;
}

bool Inventory::reduceStock(const string& id, int qty) {
    Material* p = getMaterialPtr(id);
    if (!p) return false;
    if (qty <= 0 || p->getStock() < qty) return false;
    p->setStock(p->getStock() - qty);
    return true;
}

vector<Material> Inventory::searchByName(const string& key) const {
    vector<Material> res;
    for (const auto& m : items) {
        if (m.getName().find(key) != string::npos) res.push_back(m);
    }
    return res;
}

void Inventory::listAll() const {
    cout << left << setw(10) << "ID" << left << setw(25) << "Ten" << left << setw(8) << "Don vi"
         << right << setw(10) << "Gia" << right << setw(8) << "Ton" << "\n";
    cout << string(61, '-') << "\n";
    for (const auto& m : items) m.printTableRow();
}

bool Inventory::saveToFile(const string& filename) const {
    ofstream ofs(filename);
    if (!ofs) return false;
    for (const auto& m : items) {
        ofs << m.serialize() << "\n";
    }
    return true;
}

bool Inventory::loadFromFile(const string& filename) {
    ifstream ifs(filename);
    if (!ifs) return false;
    items.clear();
    string line;
    while (getline(ifs, line)) {
        Material m;
        if (Material::deserialize(line, m)) items.push_back(m);
    }
    return true;
}
