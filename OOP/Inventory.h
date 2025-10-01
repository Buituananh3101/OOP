#ifndef INVENTORY_H
#define INVENTORY_H

#include <vector>
#include <string>
#include "Material.h"
using namespace std;

class Inventory {
private:
    vector<Material> items;
public:
    // thêm, xóa, cập nhật
    bool addMaterial(const Material& m);
    bool removeMaterial(const string& id);
    bool updateMaterial(const string& id, const string& name, const string& unit, double price, int stock);

    // nhập/xuất kho
    bool restock(const string& id, int qty);
    bool reduceStock(const string& id, int qty);

    // tìm kiếm
    int findIndexById(const string& id) const;
    Material* getMaterialPtr(const string& id);
    vector<Material> searchByName(const string& key) const;

    // hiển thị
    void listAll() const;

    // lưu/đọc file
    bool saveToFile(const string& filename) const;
    bool loadFromFile(const string& filename);
};

#endif
