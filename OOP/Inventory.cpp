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
    if (key.empty()) return res;

    // Nhập độ chính xác
    double threshold;
    cout << "Nhap do chinh xac tu 0.0 den 1.0: ";
    while (!(cin >> threshold) || threshold < 0.0 || threshold > 1.0) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Nhap lai do chinh xac (0.0 - 1.0): ";
    }

    // helper: lowercase
    auto lower = [](const string &s) {
        string t = s;
        for (auto &c : t) c = (char)tolower((unsigned char)c);
        return t;
    };

    string k = lower(key);

    auto lcs_len = [](const string &a, const string &b) {
        int n = (int)a.size(), m = (int)b.size();
        if (n == 0 || m == 0) return 0;
        vector<int> prev(m+1, 0), cur(m+1, 0);
        for (int i = 1; i <= n; ++i) {
            for (int j = 1; j <= m; ++j) {
                if (a[i-1] == b[j-1]) cur[j] = prev[j-1] + 1;
                else cur[j] = std::max(prev[j], cur[j-1]);
            }
            swap(prev, cur);
            fill(cur.begin(), cur.end(), 0);
        }
        return prev[m];
    };

    for (const auto& mtr : items) {
        string name = lower(mtr.getName());
        
        if (name.find(k) != string::npos) {
            res.push_back(mtr);
            continue;
        }
        int common = lcs_len(k, name);
        double sim = (double)common / (double)name.size(); // dùng độ dài name, không phải keyword
        if (sim >= threshold) res.push_back(mtr);
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
